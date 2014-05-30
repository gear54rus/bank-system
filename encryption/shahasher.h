#ifndef SHAHASHER_H
#define SHAHASHER_H

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>
#include <cryptopp/md5.h>

USING_NAMESPACE(CryptoPP)

class shaHasher
{
public:
    shaHasher();
    static CryptoPP::SecByteBlock hash(byte* data, int length);
};

#endif // SHAHASHER_H
