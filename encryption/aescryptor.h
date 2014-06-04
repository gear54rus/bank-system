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
    aescryptor(const QByteArray& key,const QByteArray& iv);
    void setKeyWithIV(const QByteArray& key,const QByteArray& iv);
    QByteArray encrypt(const QByteArray& message);
    QByteArray decrypt(const QByteArray& encryptedMessage);
private:
    CFB_Mode< AES >::Encryption _cfbEncryption;
    CFB_Mode< AES >::Decryption _cfbDecryption;
};

#endif // AESCRYPTOR_H
