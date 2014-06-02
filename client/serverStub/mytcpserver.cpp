#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),this, SLOT(newConnection()));
    connect(this, SIGNAL(sendNumber(char)),this,SLOT(numberSender(char)));
    if(!server->listen(QHostAddress::Any, 9999))
    {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(server->errorString());
    }
    else
    {
       qDebug() << "Server started!";
    }
}
void MyTcpServer::newConnection()
{
     qDebug() << "server: new connection";
    // need to grab the socket
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    emit(sendNumber(1));
}
void MyTcpServer::numberSender(char num)
{
    qDebug() << "server: number sender ";
    QByteArray arr;
    arr.append(num);
    socket->write(arr);
    socket->waitForBytesWritten(3000);
}

void MyTcpServer::readyRead()
{
    QByteArray data = socket->readAll();
    qDebug() << "server: got message " << data;
    Sleep(2000);
    emit(sendNumber(data.at(0)+1));
}
