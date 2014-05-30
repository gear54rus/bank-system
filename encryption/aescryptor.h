#ifndef AESCRYPTOR_H
#define AESCRYPTOR_H

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>

USING_NAMESPACE(CryptoPP)

class aescryptor
{
public:
    aescryptor();
    static void encrypt(byte* key, int keyLength, byte* iv, int ivLength, byte* message, int messageLength, byte* out, int outLength);
    static void decrypt(byte* key, int keyLength, byte* iv, int ivLength, byte* message, int messageLength, byte* out, int outLength);
};

#endif // AESCRYPTOR_H
