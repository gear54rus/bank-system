#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>

#include "../encryption/aescryptor.h"
#include "../encryption/rsacryptor.h"
#include "../encryption/dhcryptor.h"
#include "../encryption/shahasher.h"

enum connectionState
{
    DISCONNECTED,
    CLIENT_HELLO_SENT,
    SERVER_HELLO_RECEIVED,
    ACK_SENT,
    SERVER_DH_RECEIVED,
    CLIENT_DH_SENT,
    CHANGE_CIPHER_SPEC_RECEIVED,
    CHECK_HASH_SENT,
    CHECK_HASH_RECEIVED,
    SECURE_CONNECTION
};

enum messageTypes
{
    ACK = 10,
    CLOSE = 11,
    CHANGE_CIPHER_SPEC = 12,
    CLIENT_HELLO = 100,
    SERVER_HELLO = 101,
    SERVER_DH_BEGIN = 110,
    CLIENT_DH_END = 111,
    DATA = 170
};

enum logMessageType
{
    DEBUG, INFO, WARNING, ERROR
};


class connection : public QObject
{
    Q_OBJECT
public:
    explicit connection(QString rsaPublicKeyPath = QString(), QString address = QString(), quint16 port = 0, QObject *parent = 0);
    ~connection();
public slots:
    void setRsaPublicKeyPath(QString rsaPublicKeyPath);
    void setAddressAndPort(QString address, quint16 port);
    void setConnection();
    bool sendData(const QByteArray message);
    void closeConnection(QString reason, bool isByServer = false);
    QByteArray getLastReceivedMessage();
signals:
    void connecionChangeSecureState(bool isSecure);
    void gotNewMessage(QByteArray message);
    void newLogMessage(logMessageType logLevel, QString logMessage);
private slots:
    void readyRead();
    void socketDisconneted();
private:
    QTcpSocket* _socket;
    aescryptor _aes;
    rsacryptor _rsa;
    shahasher _sha256;
    connectionState _connectionState;
    QString _address;
    quint16 _port;
    QByteArray _buffer;
    QByteArray lengthToLittleEndian(short int length);
    short int lengthToBigEndian(QByteArray length);
    void changeState(connectionState newState);
    bool checkMessage(const QByteArray message);
    bool sendMessage(messageTypes messageType,const QByteArray message = QByteArray());
    bool isConnectionSecured();
    void sendNewLogMessage(logMessageType logLevel, QString logMessage);
};

#endif // CONNECTION_H
