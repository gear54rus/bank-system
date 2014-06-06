#include <QCoreApplication>
#include <logger.h>
#include <client.h>
#include <clientcontroller.h>
#include <iostream>
#include <conio.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString path = "G:\\pubKey.ini";
    Client cl("hq.zion54.net", 8815, path);
    ClientController CC(&cl);
    cl.tryNextStage();
    return a.exec();
}
