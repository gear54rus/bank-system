#include "connection.h"

Connection::Connection(QTcpSocket *socket, QObject *parent) :
    QObject(parent)
{
    this->socket = socket;
    socket->setParent(this);
    state = CONNECTED;
}
