#ifndef RSACRYPTOR_H
#define RSACRYPTOR_H

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>
#include <cryptopp/rsa.h>

#include <QSettings>
#include <QByteArray>

USING_NAMESPACE(CryptoPP)

class rsacryptor
{
public:
    rsacryptor();
    ~rsacryptor();

    void getNewRandomPrivateKey() ;
    void getPublicKeyFromPrivate() ;

    void loadPrivateKeyFromFile(std::string filename);
    void savePrivateKeyToFile(std::string filename);

    void loadPublicKeyFromFile(std::string filename);
    void savePublicKeyToFile(std::string filename);

    bool isPublicKeySet();
    bool isPrivateKeySet();

    QByteArray encrypt(const QByteArray message);
    QByteArray decrypt(const QByteArray cipherMessage) ;

    QByteArray signMessage(const QByteArray message) ;
    bool verifyMessage(const QByteArray message, const  QByteArray signature) ;

    void loadPublicKeyFromFile2(std::string filename);

private:
    bool _isPrivateKeySet;
    bool _isPublicKeySet;
    bool _isSignerSet;
    bool _isVerifierSet;
    CryptoPP::RSA::PublicKey* _pubKey;
    CryptoPP::RSA::PrivateKey* _privKey;
    CryptoPP::AutoSeededRandomPool _prng;
    CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Signer* _signer;
    CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Verifier* _verifier;
    void clearPrivKeyAndSignerIfSet();
    void clearPubKeyAndVerifierIfSet();
    void setSigner();
    void setVerifier();
};

#endif // RSACRYPTOR_H
