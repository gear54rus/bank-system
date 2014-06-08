#include "mainwindow.h"
#include <QApplication>
#include <../encryption/shahasher.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    shahasher _sha256;
    QByteArray key(32,0x12);
    _sha256.setKeyToHmac(key);
    QByteArray message;
    message.append("login username password");
    QByteArray m1 = _sha256.hmac(message);
    QByteArray m2 = _sha256.hmac(message);
    QByteArray m3 = _sha256.hmac(key,message);
    QByteArray m4 = _sha256.hmac(key,message);
    w.show();

    return a.exec();
}
