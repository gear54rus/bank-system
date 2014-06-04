#include <QCoreApplication>
#include <QtTest/QtTest>

#include "rsacryptor.h"
#include "integerencoder.h"
#include "aescryptor.h"
#include "dhcryptor.h"
#include "shahasher.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    CryptoPP::RSA::PrivateKey a = rsacryptor::getNewRandomPrivateKey();
    rsacryptor::savePrivateKeyToFile("G:\\testKey",a);
    RSA::PrivateKey b = rsacryptor::loadPrivateKeyFromFile("G:\\testKey");
    RSA::PublicKey A = rsacryptor::getPublicKeyFromPrivate(a);
    RSA::PublicKey B = rsacryptor::getPublicKeyFromPrivate(b);
    QByteArray message(20,0x02);
    bool res = rsacryptor::encrypt(A,message) == rsacryptor::encrypt(B, message);
    return 1;
}
