#include "session.h"

Session::Session(Connection* connection, QObject* parent) :
    QObject(parent)
{
    this->connection = connection;
    connect(this, SIGNAL(sendData(SecByteArray*)), connection, SLOT(send(SecByteArray*)));
    connect(this, SIGNAL(closeConnection(bool)), connection, SLOT(close(bool)));
    loggedIn = false;
}
void Session::send(SecByteArray* data)
{
    emit sendData(data);
}

void Session::end(bool normal)
{
    emit closeConnection(normal);
}
