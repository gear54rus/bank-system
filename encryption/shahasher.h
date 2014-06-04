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
    static CryptoPP::SecByteBlock hash(byte* data, int length);
    static QByteArray hash(QByteArray data);
};

#endif // SHAHASHER_H
