#include "aescryptor.h"

aescryptor::aescryptor()
{
}

void aescryptor::encrypt(byte* key, int keyLength, byte* iv, int ivLength, byte* message, int messageLength, byte* out, int outLength)
{
    CFB_Mode< AES >::Encryption cfbEncryption;
    cfbEncryption.SetKeyWithIV(key, keyLength, iv);
    cfbEncryption.ProcessData(out, message, messageLength);
}

void aescryptor::decrypt(byte* key, int keyLength, byte* iv, int ivLength, byte* message, int messageLength, byte* out, int outLength)
{
    CFB_Mode<AES>::Decryption cfbDecryption(key, keyLength, iv);
    cfbDecryption.ProcessData(out, message, messageLength);
}
 QByteArray aescryptor::encrypt(QByteArray key, QByteArray iv, QByteArray message)
 {
     QByteArray out;
     out.resize(message.size());
     CFB_Mode<AES>::Encryption cfbEncryption((byte*)key.data(), key.length(), (byte*)iv.data());
     cfbEncryption.ProcessData((byte*)out.data(), (byte*)message.data(), message.length());
     return out;
 }

 QByteArray aescryptor::decrypt(QByteArray key, QByteArray iv, QByteArray encryptedMessage)
{
    QByteArray out;
    out.resize(encryptedMessage.size());
    CFB_Mode<AES>::Decryption cfbDecryption((byte*)key.data(), key.length(), (byte*)iv.data());
    cfbDecryption.ProcessData((byte*)out.data(), (byte*)encryptedMessage.data(), encryptedMessage.length());
    return out;
}
