#include "tcplistener.h"

TcpListener::TcpListener(QObject *parent) :
    QTcpServer(parent)
{
}
