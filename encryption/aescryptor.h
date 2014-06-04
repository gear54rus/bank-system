#ifndef AESCRYPTOR_H
#define AESCRYPTOR_H

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>

#include <QByteArray>

USING_NAMESPACE(CryptoPP)

class aescryptor
{
public:
    aescryptor();
    static void encrypt(byte* key, int keyLength, byte* iv, int ivLength, byte* message, int messageLength, byte* out, int outLength);
    static void decrypt(byte* key, int keyLength, byte* iv, int ivLength, byte* message, int messageLength, byte* out, int outLength);

    static QByteArray encrypt(QByteArray key, QByteArray iv, QByteArray message);
    static QByteArray decrypt(QByteArray key, QByteArray iv, QByteArray encryptedMessage);

};

#endif // AESCRYPTOR_H
