#ifndef AESCRYPTOR_H
#define AESCRYPTOR_H

#include <cryptopp/dll.h>
#include <cryptopp/aes.h>

class aescryptor
{
public:
    aescryptor();
    ~aescryptor();
    CryptoPP::Integer ecnrypt(CryptoPP::Integer& message, void &iv = null); // iv = класс иницализашн вектора
    CryptoPP::Integer decrypt(CryptoPP::Integer& cipherMessage);
};

#endif // AESCRYPTOR_H
