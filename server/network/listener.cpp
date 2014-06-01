#include "listener.h"

TcpListener::TcpListener(QObject *parent) :
    QTcpServer(parent)
{
}

void TcpListener::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    addPendingConnection(socket);
}
