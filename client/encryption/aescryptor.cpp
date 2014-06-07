#include "aescryptor.h"

aescryptor::aescryptor()
{
}

aescryptor::aescryptor(const QByteArray &key, const QByteArray &iv)
{
    setKeyWithIV(key,iv);
}

void aescryptor::setKeyWithIV(const QByteArray &key, const QByteArray &iv)
{
    _cfbEncryption.SetKeyWithIV((byte*)key.data(),key.length(),(byte*)iv.data());
    _cfbDecryption.SetKeyWithIV((byte*)key.data(),key.length(),(byte*)iv.data());
}

QByteArray aescryptor::encrypt(const QByteArray& message)
{
    QByteArray out;
    out.resize(message.size());
    _cfbEncryption.ProcessData((byte*)out.data(), (byte*)message.data(), message.length());
    return out;
}

QByteArray aescryptor::decrypt(const QByteArray& encryptedMessage)
{
    QByteArray out;
    out.resize(encryptedMessage.size());
    _cfbDecryption.ProcessData((byte*)out.data(), (byte*)encryptedMessage.data(), encryptedMessage.length());
    return out;
}
