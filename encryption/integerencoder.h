#ifndef INTEGERENCODER_H
#define INTEGERENCODER_H

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>

USING_NAMESPACE(CryptoPP)

class integerEncoder
{
public:
    integerEncoder();
    ~integerEncoder();
    static std::string integerToString(const CryptoPP::Integer data) ;
    static CryptoPP::Integer stringToInteger(const std::string data) ;
    static int integerToBytes(byte*& data, const CryptoPP::Integer idata) ;
    static CryptoPP::Integer bytesToInteger (const byte* data, const int length) ;
};

#endif // INTEGERENCODER_H
