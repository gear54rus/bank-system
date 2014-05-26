#ifndef RSACRYPTOR_H
#define RSACRYPTOR_H

#include <cryptopp/dll.h>
#include <cryptopp/rsa.h>

USING_NAMESPACE(CryptoPP)

class rsacryptor
{
public:
    rsacryptor();
    ~rsacryptor();
    CryptoPP::Integer encrypt(CryptoPP::RSA::PublicKey, CryptoPP::Integer& message);
    std::string encrypt(CryptoPP::RSA::PublicKey, std::string& message);
    CryptoPP::Integer encrypt(CryptoPP::RSA::PublicKey, byte* message, int messageLength);
    CryptoPP::Integer decrypt(CryptoPP::RSA::PrivateKey, CryptoPP::Integer& cipherMessage);
    std::string decrypt(CryptoPP::RSA::PrivateKey, std::string& cipherMessage);
    CryptoPP::Integer decrypt(CryptoPP::RSA::PrivateKey, byte* cipherMessage, int cipherMessageLength);

    CryptoPP::RSA::PrivateKey getNewRandomPrivateKey(int keyLength);
    CryptoPP::RSA::PublicKey getPublicKeyFromPrivate(CryptoPP::RSA::PrivateKey);

    CryptoPP::Integer signMessage(CryptoPP::Integer& message);
    std::string signMessage(std::string& message);
    bool verifyMessage(CryptoPP::Integer& message);
};

#endif // RSACRYPTOR_H
