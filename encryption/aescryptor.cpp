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
