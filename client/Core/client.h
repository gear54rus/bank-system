#ifndef CLIENT_H
#define CLIENT_H


#include <QObject>
#include <QTcpSocket>


#include <logger.h>

#include "../encryption/aescryptor.h"
#include "../encryption/rsacryptor.h"
#include "../encryption/dhcryptor.h"
#include "../encryption/shahasher.h"

enum clientState
{
    clS_DISCONNECTED,
    clS_CLIENT_HELLO_SENT,
    clS_SERVER_HELLO_RECEIVED,
    clS_ACK_SENT,
    clS_SERVER_DH_RECEIVED,
    clS_CLIENT_DH_SENT,
    clS_CHANGE_CIPHER_SPEC_RECEIVED,
    clS_CHECK_HASH_SENT,
    clS_CHECK_HASH_RECEIVED,
    clS_SECURE_CONNECTION
};


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QString address, quint16 port, QString pubKeyPath, QObject *parent = 0);
    void tryToSetConnection();
    bool isConnectionSecure();

signals:
    void gotNewMessage(QString descrMessage);
    void stateChanged();
public slots:
    void tryNextStage();
private slots:
    void readyRead();
    void logNewState();
private:
    logger _log;
    clientState _connectionState;
    rsacryptor _rsa;
    aescryptor _aes;
    QByteArray _aesKey;
    QTcpSocket* _socket;
    QString _address;
    quint16 _port;
    QByteArray _dhPrivKey;
    QByteArray _dhPubKey;
    QByteArray _buffer;
    QByteArray lengthToLittleEndian(short int length);
    short int lengthToBigEndian(QByteArray length);
    void changeState(clientState newState);
    void closeConnection(bool isByServer = false);
};

#endif // CLIENT_H
