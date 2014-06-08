#include <QCoreApplication>
#include <logger.h>
#include <iostream>
#include <conio.h>
#include <connection.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString path = "G:\\pubKey.ini";
    connection n( path, "192.168.0.31", 8815);
    n.setConnection();
    return a.exec();
}
