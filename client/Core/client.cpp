#include "client.h"

Client::Client(QString address, quint16 port, QString pubKeyPath, QObject *parent) :
    QObject(parent)
{
    _socket = new QTcpSocket(this);
    _connectionState = DISCONNECTED;
    connect(_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this, SIGNAL(stateChanged()), this, SLOT(logNewState()));
    _address = address;
    _port = port;
    _rsa.loadPublicKeyFromFile2(pubKeyPath.toStdString());
    dhcryptor::initialize();
    _log << "client started";
}

void Client::readyRead()
{
    int type;
    _log << "Client got message";
    QByteArray data = _socket->readAll();
    _buffer.append(data);
    //if need to close
    if (data[0] == 11)
    {
        closeConnection(true);
        return;
    }
    switch (_connectionState)
    {
    case CLIENT_HELLO_SENT:
    {
        type = data[0];
        int protocol = data[1];
        if ((type != 101) || (protocol != 1))
        {
            closeConnection();
            return;
        }
        else
        {
            _connectionState = SERVER_HELLO_RECEIVED;
        }
        emit (stateChanged());
    }
        break;
    case ACK_SENT:
    {
        //check type
        type = data[0];
        //len of server pubkey dh
        data.remove(0,1);
        QByteArray dlen = data.left(2);
        int length = lengthToBigEndian(dlen);
        data.remove(0,2);
        //server dh pubkey
        QByteArray serverDHpublicKey = data.left(256);
        dhcryptor::getKeyPair(_dhPrivKey,_dhPubKey);
        QByteArray sharedSecret = dhcryptor::getSharedSecret(_dhPrivKey,serverDHpublicKey);
        data.remove(0,256);
        //rsa sign
        QByteArray sign = data;
        bool verResult = _rsa.verifyMessage(serverDHpublicKey,sign);
        if ((type !=110) || (verResult != true))
        {
            _log << QString("SERVER_DH_BEGIN failed: expect type = 110, verResult = true, but type = %1, verResult = %2").arg(type,verResult);
            closeConnection();
            return;
        }
        _aesKey = sharedSecret.left(32);
        _aes.setKeyWithIV(_aesKey, shahasher::hash(sharedSecret));
        changeState(SERVER_DH_RECEIVED);
    }
        break;
    case CLIENT_DH_SENT:
    {
        type = data[0];
        if (type != 12)
        {
            _log << QString("CHANGE_CIPHER_CPEC failed : expect type = 12, but type = %1").arg(type);
            closeConnection();
            return;
        }
        changeState(CHANGE_CIPHER_SPEC_RECEIVED);
    }
        break;
    case CHECK_HASH_SENT:
    {
        data = _aes.decrypt(data);
        QByteArray orData = data;
        orData.left(orData.length()-256);
        type = data[0];
        data.remove(0,1);
        QByteArray dlen = data.left(2);
        int length = lengthToBigEndian(dlen);
        data.remove(0,2);
        QByteArray concatHash = data.left(256);
        data.remove(0,256);
        QByteArray messageHmac = data;
        QByteArray ourHmac = shahasher::hmac(_aesKey,orData);
        QByteArray ourHash = shahasher::hash(_buffer);
        if ((type != 170 ) || (ourHmac != messageHmac) || (ourHash != concatHash))
        {
            _log << QString("CHECK_HASH_RECEVED failed: type = %1, messageHash = %2, ourHash = %3, messageHmac = %4, ourHmac = %5").arg(QString(type),concatHash,ourHash,messageHmac,ourHmac);
            closeConnection();
            return;
        }
        changeState(SECURE_CONNECTION);
    }
        break;

    //end of switch
    }
    if (_connectionState != DISCONNECTED)
    {
        tryNextStage();
    }
}

void Client::tryNextStage()
{
    switch (_connectionState)
    {
        case DISCONNECTED:
    {
            _socket->connectToHost(_address, _port);
            if(!_socket->waitForConnected(5000))
            {
                _log << "Client can't connect or timeouted";
                _log << _socket->errorString();
            }
            else
            {
                QByteArray clientHello;
                //type of message
                clientHello.append(100);
                //length of data
                short int len = 1;
                clientHello.append(lengthToLittleEndian(len));
                // number of our protocol (1)
                clientHello.append(1);
                _buffer.append(clientHello);
                _socket->write(clientHello);
                changeState(CLIENT_HELLO_SENT);
            }
    }
            break;
        case SERVER_HELLO_RECEIVED:
    {
            QByteArray ack;
            //type of message
            ack.append(10);
            _buffer.append(ack);
            _socket->write(ack);
            changeState(ACK_SENT);
    }
        break;
    case SERVER_DH_RECEIVED:
    {
        QByteArray message;
        message.append(111);
        message.append(lengthToLittleEndian(256));
        message.append(_dhPubKey);
        //
        //
        //
        //
        //
        //to delete
        //message.append(7);
        _buffer.append(message);
        _socket->write(message);
        changeState(CLIENT_DH_SENT);
    }
        break;
    case CHANGE_CIPHER_SPEC_RECEIVED:
    {
        QByteArray message;
        message.append(170);
        message.append(lengthToLittleEndian(256));
        message.append(shahasher::hash(_buffer));
        message.append(shahasher::hmac(_aesKey,message));
        message = _aes.encrypt(message);
        _buffer.append(message);
        _socket->write(message);
        changeState(CHECK_HASH_SENT);
    }
        break;

        default:
    {
            _log << "default";
    }
            break;
    //end of switch
    }
}


void Client::logNewState()
{
    QString mess;
    switch (_connectionState)
    {
    case DISCONNECTED:
        mess = "client state: disconnected";
        break;
    case CLIENT_HELLO_SENT:
        mess = "client state: CLIENT_HELLO_SENT";
        break;
    case SERVER_HELLO_RECEIVED:
        mess = "client state: SERVER_HELLO_RECEIVED";
        break;
    case ACK_SENT:
        mess = "client state: ACK_SENT";
        break;
    case SERVER_DH_RECEIVED:
        mess = "client state: SERVER_DH_RECEIVED";
        break;
    case CLIENT_DH_SENT:
        mess = "client state: CLIENT_DH_SENT";
        break;
    case CHANGE_CIPHER_SPEC_RECEIVED:
        mess = "client state: CHANGE_CIPHER_SPEC_RECEIVED";
        break;
    case CHECK_HASH_SENT:
        mess = "client state: CHECK_HASH_SENT";
        break;
    case CHECK_HASH_RECEIVED:
        mess = "client state: CHECK_HASH_RECEIVED";
        break;
    case SECURE_CONNECTION:
        mess = "client state: SECURE_CONNECTION";
        break;
    }
    _log << mess;
}

QByteArray Client::lengthToLittleEndian(short int length)
{
    QByteArray res;
    res.append(length/256);
    res.append(length%256);
    return res;
}

void Client::changeState(clientState newState)
{
    _connectionState = newState;
    emit(stateChanged());
}


short int Client::lengthToBigEndian(QByteArray length)
{
    short int result = length[0]*256 + length[1];
    return result;
}

void Client::closeConnection(bool isByServer)
{
    changeState(DISCONNECTED);
    _buffer.resize(0);
    if (!isByServer)
    {
        QByteArray message(1,11);
        _socket->write(message);
        _log << "client closed connection";
    }
    else
    {
        _log << "server closed connection";
    }
    _socket->close();
}
