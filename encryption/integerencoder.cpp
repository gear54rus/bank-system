#include "integerencoder.h"

integerEncoder::integerEncoder()
{
}

integerEncoder::~integerEncoder()
{
}


std::string integerEncoder::integerToString(const CryptoPP::Integer data)
{
    size_t datalength = data.MinEncodedSize();
    byte* result = new byte[datalength+1];
    memset(result,0,datalength+1);
    data.Encode((byte *)result, datalength);
    std::string resString((char*)result);
    return resString;
}

CryptoPP::Integer integerEncoder::stringToInteger(const std::string data)
{
    CryptoPP::Integer result((const byte* )data.data(),data.size());
    return result;
}

int integerEncoder::integerToBytes(byte*& data, const CryptoPP::Integer idata)
{
    size_t datalength = idata.MinEncodedSize();
    data = new byte[datalength];
    idata.Encode((byte *)data, datalength);
    return datalength;
}
CryptoPP::Integer integerEncoder::bytesToInteger (const byte* data, const int length)
{
    CryptoPP::Integer result(data, length);
    return result;
}

QByteArray integerEncoder::integerToQByteArray(const CryptoPP::Integer data)
{
    QByteArray arr;
    arr.resize(data.MinEncodedSize());
    data.Encode((byte*)arr.data(),arr.length());
    return arr;
}

CryptoPP::Integer integerEncoder::qByteArrayToInteger(const QByteArray data)
{
    CryptoPP::Integer result((byte*)data.data(),data.length());
    return result;
}
