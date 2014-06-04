#include "rsacryptor.h"
#include "integerencoder.h"

USING_NAMESPACE(CryptoPP)

rsacryptor::rsacryptor()
{
}
rsacryptor::~rsacryptor()
{
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

CryptoPP::RSA::PrivateKey rsacryptor::loadPrivateKeyFromFile(std::string filename)
{
    FileSource file(filename.c_str(), true /*pumpAll*/);
    ByteQueue queue;
    file.TransferTo(queue);
    queue.MessageEnd();
    CryptoPP::RSA::PrivateKey privKey;
    privKey.Load(queue);
    return privKey;
}

void rsacryptor::savePrivateKeyToFile(std::string filename, RSA::PrivateKey& privKey)
{
    ByteQueue queue;
    privKey.Save(queue);
    FileSink file(filename.c_str());
    queue.CopyTo(file);
    file.MessageEnd();
}

CryptoPP::RSA::PublicKey loadPublicKeyFromFile(std::string filename)
{
    FileSource file(filename.c_str(), true /*pumpAll*/);
    ByteQueue queue;
    file.TransferTo(queue);
    queue.MessageEnd();
    CryptoPP::RSA::PublicKey pubKey;
    pubKey.Load(queue);
    return pubKey;
}

CryptoPP::RSA::PublicKey savePublicKeyToFile(std::string filename, RSA::PublicKey& pubKey)
{
    ByteQueue queue;
    pubKey.Save(queue);
    FileSink file(filename.c_str());
    queue.CopyTo(file);
    file.MessageEnd();
}
