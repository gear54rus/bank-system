#include "connection.h"

namespace MSG
{
namespace
{
char clientHello[] = {0x64, 0x00, 0x01, 0x01};
char serverHello[] = {0x65, 0x01};
}
const SecByteArray ACK(1, 0x0A);
const SecByteArray CLOSE(1, 0x0B);
const SecByteArray CCS(1, 0x0C);
const SecByteArray CLIENT_HELLO(clientHello, 4);
const SecByteArray SERVER_HELLO(serverHello, 2);
}

CryptoPP::DH Connection::DH;
CryptoPP::RSA::PrivateKey Connection::RSAPrivate;
CryptoPP::AutoSeededRandomPool Connection::rnd;

void Connection::init(const SecByteArray& RSAPrivate)
{
    DH.AccessGroupParameters().Initialize(CryptoPP::Integer("0x87A8E61DB4B6663CFFBBD19C651959998CEEF608660DD0F25D2CEED4435E3B00E00DF8F1D61957D4FAF7DF4561B2AA3016C3D91134096FAA3BF4296D830E9A7C209E0C6497517ABD5A8A9D306BCF67ED91F9E6725B4758C022E0B1EF4275BF7B6C5BFC11D45F9088B941F54EB1E59BB8BC39A0BF12307F5C4FDB70C581B23F76B63ACAE1CAA6B7902D52526735488A0EF13C6D9A51BFA4AB3AD8347796524D8EF6A167B5A41825D967E144E5140564251CCACB83E6B486F6B3CA3F7971506026C0B857F689962856DED4010ABD0BE621C3A3960A54E710C375F26375D7014103A4B54330C198AF126116D2276E11715F693877FAD7EF09CADB094AE91E1A1597"),
                                          CryptoPP::Integer("0x8CF83642A709A097B447997640129DA299B1A47D1EB3750BA308B0FE64F5FBD3"),
                                          CryptoPP::Integer("0x3FB32C9B73134D0B2E77506660EDBD484CA7B18F21EF205407F4793A1A0BA12510DBC15077BE463FFF4FED4AAC0BB555BE3A6C1B0C6B47B1BC3773BF7E8C6F62901228F8C28CBB18A55AE31341000A650196F931C77A57F2DDF463E5E9EC144B777DE62AAAB8A8628AC376D282D6ED3864E67982428EBC831D14348F6F2F9193B5045AF2767164E1DFC967C1FB3F2E55A4BD1BFFE83B9C80D052B985D182EA0ADB2A3B7313D3FE14C8484B1E052588B9B7D2BBD2DF016199ECD06E1557CD0915B3353BBB64E0EC377FD028370DF92B52C7891428CDC67EB6184B523D1DB246C32F63078490F00EF8D647D148D47954515E2327CFEF98C582664B4C0F6CC41659"));
    if (RSAPrivate != "")
    {
        Connection::RSAPrivate.Load(CryptoPP::StringStore((const byte*)RSAPrivate.data(), (size_t)RSAPrivate.size()).Ref());
    }
    else
    {
        rnd.Reseed();
        Connection::RSAPrivate.GenerateRandomWithKeySize(rnd, 2048);
    }
}

SecByteArray Connection::getRSAPrivate()
{
    SecByteArray result;
    CryptoPP::ByteQueue store;
    RSAPrivate.Save(store.Ref());
    result.resize(store.MaxRetrievable());
    store.Get(reinterpret_cast<byte*>(result.data()), result.size());
    return result;
}

SecByteArray Connection::getRSAPublic()
{
    SecByteArray result;
    CryptoPP::ByteQueue store;
    CryptoPP::RSA::PublicKey RSAPublic(RSAPrivate);
    RSAPublic.Save(store.Ref());
    result.resize(store.MaxRetrievable());
    store.Get(reinterpret_cast<byte*>(result.data()), result.size());
    return result;
}

SecByteArray Connection::serializeInt(const quint16 source)
{
    SecByteArray result;
    const char* cur = reinterpret_cast<const char*>(&source);
    result.append(cur + 1, 1);
    result.append(cur, 1);
    return result;
}

Connection::Connection(QTcpSocket *socket, QObject *parent) :
    QObject(parent)
{
    this->socket = socket;
    remote = QString("%1:%2").arg(socket->peerAddress().toString(), QSN(socket->peerPort()));
    Log(QString("Inbound connection from %1...").arg(remote), "Network", Log_Debug);
    socket->setParent(this);
    state = SERVER_HELLO;
    connect(socket, SIGNAL(readyRead()), this, SLOT(newData()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(close()));
}
void Connection::close(bool normal)
{
    state = DISCONNECTED;
    if (normal)
    {
        Log(QString("Connection with remote host at %1 ened OK.").arg(remote), "Network");
    }
    else
    {
        Log(QString("Connection with %1 terminated abnormally!").arg(remote), "Network", Log_Error);
    }
    socket->disconnect(SIGNAL(disconnected()));
    socket->close();
    emit disconnected();
}

bool Connection::checkData()
{
    switch (state)
    {
    case SERVER_HELLO:
    {
        if (buffer == MSG::CLIENT_HELLO)
        {
            Log(QString("Client Hello received from %1.").arg(remote), "Network", Log_Debug);
            return true;
        }
        else
        {
            Log(QString("Bad Client Hello received from %1!").arg(remote), "Network", Log_Error);
            return false;
        }
    }
    case SERVER_DH_BEGIN:
    {
        if (buffer == MSG::ACK)
        {
            Log(QString("Protocol Acknowledge received from %1.").arg(remote), "Network", Log_Debug);
            return true;
        }
        else
        {
            Log(QString("Bad Protocol Acknowledge received from %1!").arg(remote), "Network", Log_Error);
            return false;
        }
        break;
    }
    case CCS:
    {
        break;
    }
    case VERIFY:
    {
        break;
    }
    case SECURE:
    {
        break;
    }
    case DISCONNECTED:
    {
        Q_UNREACHABLE();
    }
    }
}
void Connection::continueHandshake()
{
    switch (state)
    {
    case SERVER_HELLO:
    {
        buffer = MSG::SERVER_HELLO;
        Log(QString("Server hello sent to %1.").arg(remote), "Network", Log_Debug);
        break;
    }
    case SERVER_DH_BEGIN:
    {
        buffer.resize(0);
        buffer.append(0x6E);
        DHPrivate.resize(DH.PrivateKeyLength());
        DHPublic.resize(DH.PublicKeyLength());
        CryptoPP::SecByteBlock privateKey(DHPrivate.length()),
                 publicKey(DHPublic.length());
        DH.GenerateKeyPair(rnd, privateKey, publicKey);
        memcpy(DHPrivate.data(), privateKey.BytePtr(), DHPrivate.length());
        memcpy(DHPublic.data(), publicKey.BytePtr(), DHPublic.length());
        buffer.append(serializeInt(DHPublic.length()));
        buffer.append(DHPublic);
        CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Signer signer(RSAPrivate);
        size_t length = signer.MaxSignatureLength();
        CryptoPP::SecByteBlock signature(length);
        length = signer.SignMessage(rnd, (byte*) DHPublic.data(),  DHPublic.length(), signature);
        signature.resize(length);
        SecByteArray signatureSBA(length, 0);
        memcpy(signatureSBA.data(), signature.BytePtr(), length);
        buffer.append(signatureSBA);
        Log(QString("DH public key sent to %1.").arg(remote), "Network", Log_Debug);
        break;
    }
    case CCS:
    {
        break;
    }
    case VERIFY:
    {
        break;
    }
    case SECURE:
    {
        break;
    }
    case DISCONNECTED:
    {
        Q_UNREACHABLE();
    }
    }
    state = static_cast<STATE>(static_cast<quint8>(state) + 1);
}

SecByteArray* Connection::getPlainText()
{
    SecByteArray* result = new SecByteArray();
    return result;
}

void Connection::newData()
{
    buffer.fill(0);
    buffer = socket->readAll();
    Log(QString("(%1) %2 total").arg(QString(buffer.toHex()), QSN(buffer.size())), "Network", Log_Debug);
    if (state != SECURE)
    {
        if (!checkData())
        {
            Log(QString("Handshake with %1 failed: Bad message from client!").arg(remote), "Network", Log_Error);
            close();
        }
        else
        {
            handshake += buffer;
            buffer.fill(0);
            continueHandshake();
            socket->write(buffer);
        }
    }
    else
    {
        if (!checkData())
        {
            Log(QString("Terminating authenticated connection with %1: Bad message from client!").arg(remote), "Network", Log_Error);
            close();
        }
        else
        {
            emit received(getPlainText());
        }
    }
}
