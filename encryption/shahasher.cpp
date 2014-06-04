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


QByteArray shahasher::hash(QByteArray data)
{
    CryptoPP::SHA256 hasher;
    CryptoPP::SecByteBlock digest(32);
    hasher.CalculateDigest(digest,(byte*)data.data(),data.length());
    QByteArray dig(32,0);
    memcpy(dig.data(),digest.BytePtr(),32);
    return dig;
}
