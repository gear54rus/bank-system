#include <QCoreApplication>
#include <QtTest/QtTest>

#include "rsacryptor.h"
#include "integerencoder.h"
#include "aescryptor.h"
#include "dhcryptor.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    {
    //тестим RSA

    RSA::PrivateKey privKey = rsacryptor::getNewRandomPrivateKey();
    RSA::PublicKey pubKey = rsacryptor::getPublicKeyFromPrivate(privKey);
    RSA::PublicKey secondPubKey = rsacryptor::getPublicKeyFromPrivate(privKey);
    RSA::PrivateKey secondPrivKey = rsacryptor::getNewRandomPrivateKey();
    RSA::PublicKey pubKeyFromSecondPrivKey = rsacryptor::getPublicKeyFromPrivate(secondPrivKey);
    std::string stringMessage = "secret stringMessage";
    CryptoPP::Integer message = integerEncoder::stringToInteger(stringMessage);
    CryptoPP::Integer cipherMessage = rsacryptor::encrypt(pubKey,message);
    // зашифрованное отличается от нешифрованного
    cout << "original message == encrypted message ? false! : " << (message == cipherMessage) << endl;
    CryptoPP::Integer recoveredMessage = rsacryptor::decrypt(privKey, cipherMessage);
    // проверка расшифрованного
    cout << "original message == decrypted message && decrypted message != encrypted message ? true! :" << ((message == recoveredMessage) && (recoveredMessage != cipherMessage)) << endl;

    std::string stringMessageAfter = integerEncoder::integerToString(recoveredMessage);

    cout << "string message before: " << stringMessage << endl;
    cout << "string message after: " << stringMessageAfter << endl;

    // пабкеи с одного приватного одинаковы
    CryptoPP::Integer messageEncryptedBySecondPubKey = rsacryptor::encrypt(secondPubKey,messageEncryptedBySecondPubKey);
    cout << "message encrypted by second key == message encrypted by frist key? : true!  " << (messageEncryptedBySecondPubKey == cipherMessage) << endl;

    // шифрование с разных пар ключей - разное
    CryptoPP::Integer messageEncryptedByPubKeyFromSecondPrivKey = rsacryptor::encrypt(pubKeyFromSecondPrivKey,messageEncryptedByPubKeyFromSecondPrivKey);
    cout << "message encrypted by second key pair != message encrypted by frist key pair? : true!  " << (messageEncryptedByPubKeyFromSecondPrivKey != cipherMessage) << endl;

    //проверяем подпись
    SecByteBlock signature = rsacryptor::signMessage(message,privKey);
    SecByteBlock signatureFromSecondKey = rsacryptor::signMessage(message,secondPrivKey);
    cout << "signature != signature from second key? true: " << (signature != signatureFromSecondKey) << endl;
    cout << "verify first signature with 1st key : true :" << rsacryptor::verifyMessage(message,pubKey,signature) << endl;
    cout << "verify second signature with 1st key: false: " << rsacryptor::verifyMessage(message,pubKey,signatureFromSecondKey) << endl;
    }

    {
    // AES тестирование
    cout << "AES TESTING" << endl;
    char message[] = "hello it's test message !!";
    cout << "length of test message: " << sizeof(message) + 1 << endl;
    cout << "test message: " << message << endl;
    byte firstKey[] = "testtesttesttesttesttesttesttes";
    byte secondKey[] = "testtesttesttesttesttesttest111";
    byte iv1 [] = "123456789012345";
    byte iv2 [] = "098765432112345";
    byte* out = new byte[sizeof(message)+1];
    aescryptor::encrypt(firstKey,32,iv1,16,(byte*) message,sizeof(message) + 1,out, sizeof(message)+1);
    out[sizeof(message)]=0;
    cout << hex << out << endl;
    aescryptor::decrypt(firstKey,32,iv1,16,out ,sizeof(message)+1,out, sizeof(message)+1);
    cout << "recovered message: " <<(char*) out << endl;

    // шифрование от разных iv  должно быть разным
    aescryptor::encrypt(firstKey,32,iv1,16,(byte*) message,sizeof(message) + 1,out, sizeof(message)+1);
    out[sizeof(message)]=0;
    cout << "encrypted with 1st iv:  " << hex << out << endl;
    aescryptor::encrypt(firstKey,32,iv2,16,(byte*) message,sizeof(message) + 1,out, sizeof(message)+1);
    out[sizeof(message)]=0;
    cout << "encrypted with 2nd iv:  " << hex << out << endl;

    // шифрование от разных ключей должно быть разным
    aescryptor::encrypt(firstKey,32,iv1,16,(byte*) message,sizeof(message) + 1,out, sizeof(message)+1);
    out[sizeof(message)]=0;
    cout << "encrypted with 1st key:  " << hex << out << endl;
    aescryptor::encrypt(secondKey,32,iv1,16,(byte*) message,sizeof(message) + 1,out, sizeof(message)+1);
    out[sizeof(message)]=0;
    cout << "encrypted with 2nd key:  " << hex << out << endl;

    // iv должен изменится?
    aescryptor::encrypt(firstKey,32,iv1,16,(byte*) message,sizeof(message) + 1,out, sizeof(message)+1);
    out[sizeof(message)]=0;
    cout << "encrypted 1st time:  " << hex << out << endl;
    aescryptor::encrypt(firstKey,32,iv1,16,(byte*) message,sizeof(message) + 1,out, sizeof(message)+1);
    out[sizeof(message)]=0;
    cout << "encrypted 2nd time:  " << hex << out << endl;

    }

    {
    // DH Тестирование!
    cout << "DH testing!" << endl;
    dhcryptor::initialize();
    CryptoPP::SecByteBlock pubKey1 = dhcryptor::getPubKey();
    CryptoPP::SecByteBlock pubKey2 = dhcryptor::getPubKey();
    CryptoPP::SecByteBlock privKey1 = dhcryptor::getPrivKey();
    CryptoPP::SecByteBlock privKey2 = dhcryptor::getPrivKey();
    dhcryptor::setKeyPair(privKey1,pubKey1);
    dhcryptor::setKeyPair(privKey2,pubKey2);
    CryptoPP::SecByteBlock secret1 = dhcryptor::getSharedSecret(privKey1,pubKey2);
    CryptoPP::SecByteBlock secret2 = dhcryptor::getSharedSecret(privKey2,pubKey1);
    cout << "first key pair: " << endl << "pubkey: " << hex << pubKey1.data() << endl << "privKey: "  << privKey1.data() << endl;
    cout << "second key pair: " << endl << "pubkey: " << hex << pubKey2.data() << endl << "privKey: "  << privKey2.data() << endl;
    cout << "are pub keys equal? false : " << ((pubKey1.size() == pubKey2.size()) && (0 == memcmp(pubKey1.BytePtr(), pubKey2.BytePtr(), pubKey1.size()))) << endl;
    cout << "are priv keys equal? false : " << ((privKey1.size() == privKey2.size()) && (0 == memcmp(privKey1.BytePtr(), privKey2.BytePtr(), privKey1.size()))) << endl;
    cout << "shared secret1: " << hex << secret1.data() << endl;
    cout << "shared secret2: " << hex  << secret2.data() << endl;
    cout << "are shared secrets equal? true :" << ((secret1.size() == secret2.size()) && (0 == memcmp(secret1.BytePtr(), secret2.BytePtr(), secret1.size()))) << endl;
    }
}
