#include <QCoreApplication>
#include <iostream>
#include <QTcpServer>
#include <mytcpserver.h>
#include <mytcpclient.h>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MyTcpServer serv;
    MyTcpClient client;
    client.startConnection("127.0.0.1",9999);
    return a.exec();
}

