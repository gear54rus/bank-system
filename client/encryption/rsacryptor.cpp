#include "rsacryptor.h"

USING_NAMESPACE(CryptoPP)

rsacryptor::rsacryptor()
{
    _isPrivateKeySet = false;
    _isPublicKeySet = false;
    _isSignerSet = false;
    _isVerifierSet = false;
}
rsacryptor::~rsacryptor()
{
   clearPubKeyAndVerifierIfSet();
   clearPrivKeyAndSignerIfSet();
}

void rsacryptor::setSigner()
{
    if (_isSignerSet) delete _signer;
    if (_isPrivateKeySet)
    {
         _signer = new RSASS<PSS, SHA256>::Signer(*_privKey);
         _isSignerSet = true;
    }
    else
        _isSignerSet = false;
}

void rsacryptor::setVerifier()
{
    if (_isVerifierSet) delete _verifier;
    if (_isPublicKeySet)
    {
         _verifier = new CryptoPP::RSASS < CryptoPP::PSS, CryptoPP::SHA256>::Verifier(*_pubKey);
         _isVerifierSet = true;
    }
    else
        _isVerifierSet = false;
}

void rsacryptor::getNewRandomPrivateKey()
{
    clearPrivKeyAndSignerIfSet();
    _privKey = new CryptoPP::RSA::PrivateKey();
    _prng.Reseed();
    _privKey->GenerateRandomWithKeySize(_prng, 2048);
    _isPrivateKeySet = true;
    setSigner();

}

void rsacryptor::getPublicKeyFromPrivate()
{
    clearPubKeyAndVerifierIfSet();
    _pubKey = new CryptoPP::RSA::PublicKey(*_privKey);
    _isPublicKeySet = true;
    setVerifier();

}

QByteArray rsacryptor::encrypt(const QByteArray message)
{
    if (_isPublicKeySet == false) return QByteArray();
    CryptoPP::Integer m((byte*)message.data(),message.length());
    m = _pubKey->ApplyFunction(m);
    QByteArray result(m.MinEncodedSize(),0);
    m.Encode((byte*)result.data(),result.length());
    return result;
}

QByteArray rsacryptor::decrypt(const QByteArray cipherMessage)
{
    if (_isPrivateKeySet == false) return QByteArray();
    CryptoPP::Integer m((byte*)cipherMessage.data(),cipherMessage.length());
    m = _privKey->CalculateInverse(_prng , m);
    QByteArray result(m.MinEncodedSize(),0);
    m.Encode((byte*)result.data(),result.length());
    return result;
}

QByteArray rsacryptor::signMessage(const QByteArray message)
{
    if (_isSignerSet == false) return QByteArray();

    // Create signature space
    size_t length = _signer->MaxSignatureLength();
    SecByteBlock signature(length);

    // Sign message
    length = _signer->SignMessage(_prng, (byte*) message.data(),
       message.length(), signature);

    // Resize now we know the true size of the signature
    signature.resize(length);

    QByteArray result(length,0);
    memcpy(result.data(),signature.BytePtr(),length);
    return result;
}

bool rsacryptor::verifyMessage(const QByteArray message, const  QByteArray signature)
{
    if (_isVerifierSet == false) return false;

    // Verify
    bool result = _verifier->VerifyMessage((byte*)message.data(),
       message.length(), (byte*)signature.data(), signature.size());

    // Result
   return result;
}

void rsacryptor::loadPrivateKeyFromFile(std::string filename)
{
    clearPrivKeyAndSignerIfSet();
    FileSource file(filename.c_str(), true /*pumpAll*/);
    ByteQueue queue;
    file.TransferTo(queue);
    queue.MessageEnd();
    _privKey = new CryptoPP::RSA::PrivateKey;
    _privKey->Load(queue);
    _isPrivateKeySet = true;
    setSigner();
}

void rsacryptor::savePrivateKeyToFile(std::string filename)
{
    ByteQueue queue;
    _privKey->Save(queue);
    FileSink file(filename.c_str());
    queue.CopyTo(file);
    file.MessageEnd();
}

void rsacryptor::loadPublicKeyFromFile(std::string filename)
{
    FileSource file(filename.c_str(), true /*pumpAll*/);
    ByteQueue queue;
    file.TransferTo(queue);
    queue.MessageEnd();
    clearPubKeyAndVerifierIfSet();
    _pubKey = new RSA::PublicKey;
    _pubKey->Load(queue);
    _isPublicKeySet = true;
    setVerifier();
}

void rsacryptor::savePublicKeyToFile(std::string filename)
{
    ByteQueue queue;
    _pubKey->Save(queue);
    FileSink file(filename.c_str());
    queue.CopyTo(file);
    file.MessageEnd();
}

void rsacryptor::clearPubKeyAndVerifierIfSet()
{
    if (_isPublicKeySet)
    {
        delete _pubKey;
        _isPublicKeySet = false;
    }
    if (_isVerifierSet)
    {
        delete _verifier;
        _isVerifierSet = false;
    }
}

void rsacryptor::clearPrivKeyAndSignerIfSet()
{
    if (_isPrivateKeySet)
    {
        delete _privKey;
        _isPrivateKeySet = false;
    }
    if (_isSignerSet)
    {
        delete _signer;
        _isSignerSet = false;
    }
}

bool rsacryptor::isPublicKeySet()
{
    return _isPublicKeySet;
}

bool rsacryptor::isPrivateKeySet()
{
    return _isPrivateKeySet;
}

void rsacryptor::loadPublicKeyFromFile2(std::string filename)
{
    clearPubKeyAndVerifierIfSet();
    QSettings s(filename.c_str(), QSettings::IniFormat);
    s.beginGroup("RSA");
    QByteArray key = QByteArray::fromBase64(s.value("key", "").toByteArray());
    _pubKey = new RSA::PublicKey;
    _pubKey->Load(CryptoPP::StringStore((const byte*)key.data(), (size_t)key.size()).Ref());
    _isPublicKeySet = true;
    setVerifier();
}
