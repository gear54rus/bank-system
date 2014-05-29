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
    RSASS<PSS, CryptoPP::SHA1>::Signer signer(privKey);

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
    RSASS<PSS, CryptoPP::SHA1>::Verifier verifier(pubKey);

    //Integer message to bytes
    byte* data;
    int datalength = integerEncoder::integerToBytes(data,message);

    // Verify
    bool result = verifier.VerifyMessage(data,
       datalength, signature, signature.size());

    // Result
   return result;
}
