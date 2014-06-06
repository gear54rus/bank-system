#ifndef CLIENT_H
#define CLIENT_H


#include <QObject>
#include <QTcpSocket>


#include <logger.h>

#include "../../encryption/aescryptor.h"
#include "../../encryption/rsacryptor.h"
#include "../../encryption/dhcryptor.h"
#include "../../encryption/shahasher.h"

enum clientState
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


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QString address, quint16 port, QString pubKeyPath, QObject *parent = 0);
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
