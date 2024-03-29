#include "connection.h"

connection::~connection()
{
    _socket->close();
    delete _socket;
}

connection::connection(QString rsaPublicKeyPath, QString address , quint16 port, QObject *parent):
    QObject(parent)
{
    _socket = new QTcpSocket(this);
    _connectionState = DISCONNECTED;
    connect(_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(_socket, SIGNAL(disconnected()), this, SLOT(socketDisconneted()));
    setAddressAndPort(address, port);
    setRsaPublicKeyPath(rsaPublicKeyPath);
    dhcryptor::initialize();
}

void connection::readyRead()
{
    QByteArray data = _socket->readAll();
    while (data.size())
    {
        QByteArray message;
        int messageLength = 0;
        unsigned char dataType = data[0];
        switch (dataType)
        {
        case ACK:
            messageLength = 1;
            message = data.left(messageLength);
            data.remove(0,messageLength);
            break;
        case CLOSE:
            messageLength = 1;
            message = data.left(messageLength);
            data.remove(0,messageLength);
            break;
        case CHANGE_CIPHER_SPEC:
            messageLength = 1;
            message = data.left(messageLength);
            data.remove(0,messageLength);
            break;
        case SERVER_HELLO:
            messageLength = 2;
            message = data.left(messageLength);
            data.remove(0,messageLength);
            break;
        case SERVER_DH_BEGIN:
            messageLength = 1 + 2 + 256 + 256;
            message = data.left(messageLength);
            data.remove(0,messageLength);
            break;
        case DATA:
            messageLength = 1 + 2 + lengthToBigEndian(data.mid(1,2)) + 32;
            message = data.left(messageLength);
            data.remove(0,messageLength);
            break;
        default:
            data.clear();
            closeConnection("received unknown type of message");
        }
        if (messageLength)
        {
            processNewMessage(message);
        }
    }
}

void connection::processNewMessage(QByteArray message)
{
    QString checkMessageResult = checkMessage(message);
    if (!checkMessageResult.isEmpty())
    {
        closeConnection("received corrupted message: " + checkMessageResult);
        return;
    }
    unsigned char type = message[0];
    if (type == CLOSE)
    {
        closeConnection("",true);
        return;
    }
    if (_connectionState == SECURE_CONNECTION)
    {
        if (type != DATA)
        {
            sendNewLogMessage(ERROR, "got corrupted message in secure connection");
            return;
        }
        else
        {
            short unsigned int length = lengthToBigEndian(message.mid(1,2));
            _buffer = message.mid(3,length);
            _buffer = _aes.decrypt(_buffer);
            emit( gotNewMessage(_buffer));
        }
    }
    else
    {
        switch (_connectionState)
        {
        case CLIENT_HELLO_SENT:
        {
            if (type != SERVER_HELLO)
            {
                closeConnection("received message was not of server hello type after client hello");
                return;
            }
            changeState(SERVER_HELLO_RECEIVED);
            _buffer.append(message);
            sendMessage(ACK);
            changeState(ACK_SENT);

        }
            break;
        case ACK_SENT:
        {
            if (type != SERVER_DH_BEGIN)
            {
                closeConnection("received message was not of server dh begin type after ack");
                return;
            }
            changeState(SERVER_DH_RECEIVED);
            short unsigned int dataLength = lengthToBigEndian(message.mid(1,2));
            QByteArray serverDhPublicKey = message.mid(3,dataLength);
            QByteArray myDhPrivateKey, myDhPublicKey;
            dhcryptor::getKeyPair(myDhPrivateKey,myDhPublicKey);
            QByteArray sharedSecret = dhcryptor::getSharedSecret(myDhPrivateKey,serverDhPublicKey);
            QByteArray _aesKey = sharedSecret.left(32);
            QByteArray iv = _sha256.hash(sharedSecret);
            _aes.setKeyWithIV(_aesKey,iv);
            _sha256.setKeyToHmac(_aesKey);
            _buffer.append(message);
            sendMessage(CLIENT_DH_END, myDhPublicKey);
            changeState(CLIENT_DH_SENT);
        }
            break;
        case CLIENT_DH_SENT:
        {
            if (type != CHANGE_CIPHER_SPEC)
            {
                closeConnection("received message was not of cipher spec type after client dh end");
                return;
            }
            changeState(CHANGE_CIPHER_SPEC_RECEIVED);
            _buffer.append(message);
            sendMessage(DATA, _sha256.hash(_buffer));
            changeState(CHECK_HASH_SENT);
        }
            break;

        case CHECK_HASH_SENT:
        {
            if (type != DATA)
            {
                closeConnection("received message was not of data type in check hash exhange");
                return;
            }
            changeState(CHECK_HASH_RECEIVED);
            short unsigned int dataLength = lengthToBigEndian(message.mid(1,2));
            QByteArray serverHash = message.mid(3,32);
            serverHash = _aes.decrypt(serverHash);
            QByteArray ourHash = _sha256.hash(_buffer);
            if (!(ourHash == serverHash))
            {
                closeConnection("check hashes failed");
            }
            changeState(SECURE_CONNECTION);
            _buffer.resize(0);
        }
            break;
        // end of switch
        }
    }

}


void connection::closeConnection(QString reason, bool isByServer)
{
    _buffer.resize(0);
    if (!isByServer)
    {
        sendMessage(CLOSE);
        sendNewLogMessage(WARNING, "client closed connection: " + reason);
    }
    else
    {
        sendNewLogMessage(WARNING, "server closed connection: " + reason);
    }
    changeState(DISCONNECTED);
    _socket->close();
}


QByteArray connection::lengthToLittleEndian(short int length)
{
    QByteArray res;
    res.append(length/256);
    res.append(length%256);
    return res;
}

short int connection::lengthToBigEndian(QByteArray length)
{
    short int result = length[0]*256 + length[1];
    return result;
}

QString connection::checkMessage(const QByteArray message)
{
    QString result;
    if (message.length() == 0) return result;
    unsigned char type = message[0];
    short unsigned int length = 0;
    switch (type)
    {
    case ACK:
        if (message.length() != 1) result += "length of ACK is not 1";
        break;
    case CLOSE:
        if (message.length( ) != 1) result += "length of CLOSE is not 1";
        break;
    case CHANGE_CIPHER_SPEC:
        if (message.length() != 1) result += "length of CHANGE_CIPHER_SPEC is not 1";
        break;
    case CLIENT_HELLO:
        length = lengthToBigEndian(message.mid(1,2));
        if (message.length() != length + 3) result += "length of CLIENT_HELLO is not 1";
        break;
    case SERVER_HELLO:
        if (message.length() != 2) result += "length of SERVER_HELLO is not 2";
        break;
    case SERVER_DH_BEGIN:
        length = lengthToBigEndian(message.mid(1,2));
        if (message.length() != (length +1+2+256)) result += "length of SERVER_DH_BEGIN is wrong";
        if (!_rsa.verifyMessage(message.mid(3,length),message.mid(259,256))) result += "RSA signature verification failed" ;
        break;
    case CLIENT_DH_END:
        length = lengthToBigEndian(message.mid(1,2));
        if (message.length() != length + 3) result += "length of CLIENT_DH_END is wrong";
        break;
    case DATA:
        length = lengthToBigEndian(message.mid(1,2));
        if (message.length() != length + 35) result += QString("length of DATA is wrong: real message length = ") + QString::number(message.length()) + " get in message = " + QString::number(length) + " ";
        if (! (_sha256.hmac(message.mid(3,length)) == message.right(32))) result += "HMAC verification failed";
        break;
    default:
        result += "dont know how to proccess this type of message";
    }
    return result;
}

bool connection::sendMessage(messageTypes messageType, const QByteArray message)
{
    QByteArray data;
    switch (messageType)
    {
    case ACK:
        data.append(10);
        break;
    case CLOSE:
        data.append(11);
        break;
    case CHANGE_CIPHER_SPEC:
        data.append(12);
        break;
    case CLIENT_HELLO:
        data.append(100);
        data.append(lengthToLittleEndian(1));
        data.append(1);
        break;
    case CLIENT_DH_END:
        data.append(111);
        data.append(lengthToLittleEndian(message.size()));
        data.append(message);
        break;
    case DATA:
    {
        data.append(170);
        QByteArray encryptedMessage = _aes.encrypt(message);
        data.append(lengthToLittleEndian(encryptedMessage.length()));
        data.append(encryptedMessage);
        data.append(_sha256.hmac(encryptedMessage));
    }
        break;
    default:
        return false;
    }

    if (!(_connectionState == SECURE_CONNECTION))
        _buffer.append(data) ;
    _socket->write(data);
}

void connection::changeState(connectionState newState)
{
    if (_connectionState == newState)
    {
        return;
    }
    QString mess;
    if ((_connectionState == SECURE_CONNECTION) && (newState != SECURE_CONNECTION))
    {
        emit (connecionChangeSecureState(false));
    }
    _connectionState = newState;
    switch (_connectionState)
    {
    case DISCONNECTED:
        mess = "connection state: disconnected";
        break;
    case CLIENT_HELLO_SENT:
        mess = "connection state: CLIENT_HELLO_SENT";
        break;
    case SERVER_HELLO_RECEIVED:
        mess = "connection state: SERVER_HELLO_RECEIVED";
        break;
    case ACK_SENT:
        mess = "connection state: ACK_SENT";
        break;
    case SERVER_DH_RECEIVED:
        mess = "connection state: SERVER_DH_RECEIVED";
        break;
    case CLIENT_DH_SENT:
        mess = "connection state: CLIENT_DH_SENT";
        break;
    case CHANGE_CIPHER_SPEC_RECEIVED:
        mess = "connection state: CHANGE_CIPHER_SPEC_RECEIVED";
        break;
    case CHECK_HASH_SENT:
        mess = "connection state: CHECK_HASH_SENT";
        break;
    case CHECK_HASH_RECEIVED:
        mess = "connection state: CHECK_HASH_RECEIVED";
        break;
    case SECURE_CONNECTION:
        mess = "connection state: SECURE_CONNECTION";
        emit (connecionChangeSecureState(true));
        break;
    }
    sendNewLogMessage(DEBUG, mess);
}

void connection::setConnection()
{
    _socket->connectToHost(_address, _port);
    if(!_socket->waitForConnected(5000))
    {
        sendNewLogMessage(ERROR, _socket->errorString());
        return;
    }
    sendMessage(CLIENT_HELLO);
    changeState(CLIENT_HELLO_SENT);
}

bool connection::isConnectionSecured()
{
    return _connectionState == SECURE_CONNECTION;
}

bool connection::sendData(const QByteArray message)
{
    if (isConnectionSecured())
    {
        sendMessage(DATA, message);
        return true;
    }
    return false;
}

QByteArray connection::getLastReceivedMessage()
{
    return _buffer;
}

void connection::setRsaPublicKeyPath(QString rsaPublicKeyPath)
{
    // instead of norm test path
    if (rsaPublicKeyPath.size() == 0)
    {
        sendNewLogMessage(ERROR, "RSA Public Key Path is not set");
        return;
    }
    if (_connectionState != DISCONNECTED)
    {
        closeConnection("set new RSA Public Key");
    }
    _rsa.loadPublicKeyFromFile2(rsaPublicKeyPath.toStdString());
}

void connection::setAddressAndPort(QString address, quint16 port)
{
    if ((address.size() == 0) || (port == 0))
    {
        sendNewLogMessage(ERROR, "Address and port are not set");
        return;
    }
    if (_connectionState != DISCONNECTED)
    {
        closeConnection("set new address and port");
    }
    _address = address;
    _port = port;
}

void connection::sendNewLogMessage(logMessageType logLevel, QString logMessage)
{
    emit(newLogMessage(logLevel, "connection: " + logMessage));
}

void connection::socketDisconneted()
{
    closeConnection("server socket was closed", true);
    _socket->close();
}
