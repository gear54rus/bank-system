#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>

#include <logger.h>

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

class connection : public QObject
{
    Q_OBJECT
public:
    explicit connection(QString rsaPublicKeyPath, QString address = QString(), quint16 port = 0, QObject *parent = 0);
    void setConnection();
    bool sendData(QByteArray message);
    QByteArray getLastReceivedMessage();
signals:
private slots:
    void readyRead();
private:
    QTcpSocket* _socket;
    aescryptor _aes;
    rsacryptor _rsa;
    QByteArray _aesKey;
    logger _log;
    connectionState _connectionState;
    QString _address;
    quint16 _port;
    QByteArray _buffer;
    QByteArray lengthToLittleEndian(short int length);
    short int lengthToBigEndian(QByteArray length);
    void changeState(connectionState newState);
    void closeConnection(QString reason, bool isByServer = false);
    bool checkMessage(const QByteArray message);
    bool sendMessage(messageTypes messageType,const QByteArray message = QByteArray());
};

#endif // CONNECTION_H
