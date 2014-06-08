#ifndef SHAHASHER_H
#define SHAHASHER_H

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>
#include <cryptopp/md5.h>

#include <QByteArray>

USING_NAMESPACE(CryptoPP)

class shahasher
{
public:
    shahasher();
    QByteArray hash(QByteArray data);
    QByteArray hmac(QByteArray data);
    QByteArray hmac(QByteArray key, QByteArray data);
    void setKeyToHmac(QByteArray key);
private:
    CryptoPP::HMAC < CryptoPP::SHA256 > hmacer;
    CryptoPP::SHA256 hasher;
};

#endif // SHAHASHER_H
