#ifndef DHCRYPTOR_H
#define DHCRYPTOR_H

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>

USING_NAMESPACE(CryptoPP)

class dhcryptor
{
public:
    dhcryptor();
    static void initialize();
    static CryptoPP::SecByteBlock getPubKey();
    static CryptoPP::SecByteBlock getPrivKey();
    static void setKeyPair(CryptoPP::SecByteBlock& privKey, CryptoPP::SecByteBlock& pubKey);
    static CryptoPP::SecByteBlock getSharedSecret(CryptoPP::SecByteBlock& privKeyA, CryptoPP::SecByteBlock& pubKeyB);
private:
    static CryptoPP::DH _dh;
};

#endif // DHCRYPTOR_H
