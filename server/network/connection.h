#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
//#define CRYPTOPP_DEFAULT_NO_DLL
//#include <cryptopp/dll.h>


class Connection: public QObject
{
public:
    explicit Connection(QTcpSocket* socket, QObject* parent = 0);
    enum STATE {SERVER_HELLO, SERVER_DH_BEGIN, CCS, VERIFY, SECURE, DISCONNECTED};
private:
    QTcpSocket* socket;
    STATE state;
};

#endif // CONNECTION_H
