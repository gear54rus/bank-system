#ifndef RSACRYPTOR_H
#define RSACRYPTOR_H

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>
#include <cryptopp/rsa.h>

#include <QByteArray>

USING_NAMESPACE(CryptoPP)

class rsacryptor
{
public:
    rsacryptor();
    ~rsacryptor();
    static CryptoPP::Integer encrypt(const CryptoPP::RSA::PublicKey pubKey, const CryptoPP::Integer& message) ;

    static CryptoPP::Integer decrypt(const CryptoPP::RSA::PrivateKey privKey, const CryptoPP::Integer& cipherMessage) ;

    static CryptoPP::RSA::PrivateKey getNewRandomPrivateKey() ;
    static CryptoPP::RSA::PublicKey getPublicKeyFromPrivate(const CryptoPP::RSA::PrivateKey privKey) ;

    static CryptoPP::SecByteBlock signMessage(const CryptoPP::Integer& message, const CryptoPP::RSA::PrivateKey privKey) ;
    static bool verifyMessage(const CryptoPP::Integer& message,const CryptoPP::RSA::PublicKey& pubKey, const  CryptoPP::SecByteBlock& signature) ;

    //методы для QByteArray
    static QByteArray encrypt(const CryptoPP::RSA::PublicKey pubKey, const QByteArray message);
    static QByteArray decrypt(const CryptoPP::RSA::PrivateKey privKey, const QByteArray cipherMessage) ;

    static QByteArray signMessage(const QByteArray message, const CryptoPP::RSA::PrivateKey privKey) ;
    static bool verifyMessage(const QByteArray message,const CryptoPP::RSA::PublicKey& pubKey, const  QByteArray signature) ;

};

#endif // RSACRYPTOR_H
