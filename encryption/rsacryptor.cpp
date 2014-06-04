#include "rsacryptor.h"
#include "integerencoder.h"

USING_NAMESPACE(CryptoPP)

rsacryptor::rsacryptor()
{
}
rsacryptor::~rsacryptor()
{
}

CryptoPP::Integer rsacryptor::encrypt(const CryptoPP::RSA::PublicKey pubKey, const CryptoPP::Integer& message)
{
     return pubKey.ApplyFunction(message);
}



CryptoPP::Integer rsacryptor::decrypt(const CryptoPP::RSA::PrivateKey privKey, const CryptoPP::Integer& cipherMessage)
{
    AutoSeededRandomPool rnd;
    return privKey.CalculateInverse(rnd , cipherMessage);
}

CryptoPP::RSA::PrivateKey rsacryptor::getNewRandomPrivateKey()
{
    AutoSeededRandomPool prng;
    CryptoPP::RSA::PrivateKey privKey;
    privKey.GenerateRandomWithKeySize(prng, 2048);
    return privKey;
}

 CryptoPP::RSA::PublicKey rsacryptor::getPublicKeyFromPrivate(const CryptoPP::RSA::PrivateKey privKey)
 {
     CryptoPP::RSA::PublicKey pubKey(privKey);
     return pubKey;
 }

 CryptoPP::SecByteBlock rsacryptor::signMessage(const CryptoPP::Integer& message, const CryptoPP::RSA::PrivateKey privKey)
 {
    AutoSeededRandomPool rng;

    // Signer object
    RSASS<PSS, CryptoPP::SHA256>::Signer signer(privKey);

    // Create signature space
    size_t length = signer.MaxSignatureLength();
    SecByteBlock signature(length);

    //Integer message to bytes
    byte* data;
    int datalength = integerEncoder::integerToBytes(data,message);

    // Sign message
    length = signer.SignMessage(rng, data,
       datalength, signature);

    // Resize now we know the true size of the signature
    signature.resize(length);

    return signature;

 }
bool rsacryptor::verifyMessage(const CryptoPP::Integer& message, const CryptoPP::RSA::PublicKey& pubKey, const CryptoPP::SecByteBlock& signature)
{
    // Verifier object
    RSASS<PSS, CryptoPP::SHA256>::Verifier verifier(pubKey);

    //Integer message to bytes
    byte* data;
    int datalength = integerEncoder::integerToBytes(data,message);

    // Verify
    bool result = verifier.VerifyMessage(data,
       datalength, signature, signature.size());

    // Result
   return result;
}

// QByteArray


QByteArray rsacryptor::encrypt(const CryptoPP::RSA::PublicKey pubKey, const QByteArray message)
{
    CryptoPP::Integer m((byte*)message.data(),message.length());
    m = pubKey.ApplyFunction(m);
    QByteArray result(m.MinEncodedSize(),0);
    m.Encode((byte*)result.data(),result.length());
    return result;
}

QByteArray rsacryptor::decrypt(const CryptoPP::RSA::PrivateKey privKey, const QByteArray cipherMessage)
{
    CryptoPP::Integer m((byte*)cipherMessage.data(),cipherMessage.length());
    AutoSeededRandomPool rnd;
    m = privKey.CalculateInverse(rnd , m);
    QByteArray result(m.MinEncodedSize(),0);
    m.Encode((byte*)result.data(),result.length());
    return result;
}

QByteArray rsacryptor::signMessage(const QByteArray message, const CryptoPP::RSA::PrivateKey privKey)
{
    AutoSeededRandomPool rng;

    // Signer object
    RSASS<PSS, CryptoPP::SHA256>::Signer signer(privKey);

    // Create signature space
    size_t length = signer.MaxSignatureLength();
    SecByteBlock signature(length);

    // Sign message
    length = signer.SignMessage(rng, (byte*) message.data(),
       message.length(), signature);

    // Resize now we know the true size of the signature
    signature.resize(length);

    QByteArray result(length,0);
    memcpy(result.data(),signature.BytePtr(),length);
    return result;
}

bool rsacryptor::verifyMessage(const QByteArray message,const CryptoPP::RSA::PublicKey& pubKey, const  QByteArray signature)
{
    // Verifier object
    RSASS<PSS, CryptoPP::SHA256>::Verifier verifier(pubKey);

    // Verify
    bool result = verifier.VerifyMessage((byte*)message.data(),
       message.length(), (byte*)signature.data(), signature.size());

    // Result
   return result;
}



