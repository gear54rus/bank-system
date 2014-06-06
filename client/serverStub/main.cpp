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
    client.startConnection("hq.zion54.net",8815);
    return a.exec();
}

