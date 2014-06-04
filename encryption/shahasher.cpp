#include "shahasher.h"

shahasher::shahasher()
{
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


QByteArray shahasher::hmac(QByteArray key, QByteArray data)
{
    CryptoPP::HMAC < SHA256 > hmacer((byte*)key.data(),key.length());
    CryptoPP::SecByteBlock digest(32);
    hmacer.CalculateDigest(digest,(byte*)data.data(),data.length());
    QByteArray dig(32,0);
    memcpy(dig.data(),digest.BytePtr(),32);
    return dig;
}
