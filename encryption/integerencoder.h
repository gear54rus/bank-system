#ifndef INTEGERENCODER_H
#define INTEGERENCODER_H

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>
#include <QByteArray>

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

    static QByteArray integerToQByteArray(const CryptoPP::Integer data);
    static CryptoPP::Integer qByteArrayToInteger(const QByteArray data);

};

#endif // INTEGERENCODER_H
