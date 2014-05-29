#include <QCoreApplication>
#include <QtTest/QtTest>

#include "rsacryptor.h"
#include "integerencoder.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
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
