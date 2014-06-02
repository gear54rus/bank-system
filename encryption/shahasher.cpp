#include "shahasher.h"

shahasher::shahasher()
{
}


CryptoPP::SecByteBlock shahasher::hash(byte* data, int length)
{
    CryptoPP::SHA256 hasher;
    CryptoPP::SecByteBlock digest(32);
    hasher.CalculateDigest(digest,data,length);
    return digest;
}
