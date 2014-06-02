#include "mytcpclient.h"

MyTcpClient::MyTcpClient(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    // whenever a user connects, it will emit signal
    connect(this, SIGNAL(sendNumber(char)),this,SLOT(numberSender(char)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    qDebug() << "client started" << endl;
}
void MyTcpClient::startConnection(QString address, quint16 port)
{
     qDebug() << "client: start connection";
    // this is not blocking call
    socket->connectToHost(address, port);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
       qDebug() << "Client error: " << socket->errorString();
    }
}

void MyTcpClient::numberSender(char num)
{
    qDebug() << "client: number sender ";
    QByteArray arr;
    arr.append(num);
    socket->write(arr);
    socket->waitForBytesWritten(3000);
}
void MyTcpClient::readyRead()
{
    QByteArray data = socket->readAll();
    qDebug() << "client: got message " << data;
    Sleep(2000);
    emit(sendNumber(data.at(0)+1));
}
