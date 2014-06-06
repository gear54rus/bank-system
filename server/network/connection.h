#ifndef CONNECTION_H
#define CONNECTION_H

#include "../crypto/secbytearray.h"
#include "../core/logger.h"
#include "../global.h"
#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>
#include <cryptopp/rsa.h>
#include <QTcpSocket>
#include <QHostAddress>

class Connection: public QObject
{
    Q_OBJECT
public:
    enum STATE {SERVER_HELLO = 1, SERVER_DH_BEGIN, CCS, VERIFY, SECURE, DISCONNECTED};
    static void init(const SecByteArray& RSAPrivate = "");
    static SecByteArray getRSAPrivate();
    static SecByteArray getRSAPublic();
    static SecByteArray serializeInt(const quint16 source);
    explicit Connection(QTcpSocket* socket, QObject* parent = 0);
    STATE getState()
    {
        return state;
    }
private:
    static CryptoPP::DH DH;
    static CryptoPP::RSA::PrivateKey RSAPrivate;
    static CryptoPP::AutoSeededRandomPool rnd;
    QTcpSocket* socket;
    STATE state;
    QString remote;
    SecByteArray buffer;
    SecByteArray handshake;
    SecByteArray DHPrivate;
    SecByteArray DHPublic;
    bool checkData();
    void continueHandshake();
    SecByteArray* getPlainText();
public slots:
    void close(bool normal = false);
private slots:
    void newData();
signals:
    void disconnected();
    void received(SecByteArray* data);
};

#endif // CONNECTION_H
