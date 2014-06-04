#ifndef DHCRYPTOR_H
#define DHCRYPTOR_H

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>

#include <QByteArray>

USING_NAMESPACE(CryptoPP)

class dhcryptor
{
public:
    dhcryptor();
    static void initialize();
    static void getKeyPair(QByteArray& privKey, QByteArray& pubKey);
    static QByteArray getSharedSecret(QByteArray& privKeyA, QByteArray& pubKeyB);
private:
    static CryptoPP::DH _dh;
};

#endif // DHCRYPTOR_H
