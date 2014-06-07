#include <QString>
#include <QtTest>
#include <../../rsacryptor.h>
#include <../../rsacryptor.cpp>
#include <../../aescryptor.h>
#include <../../aescryptor.cpp>
#include <../../dhcryptor.h>
#include <../../dhcryptor.cpp>
#include <../../shahasher.h>
#include <../../shahasher.cpp>

USING_NAMESPACE(CryptoPP)

class EncryptionTests : public QObject
{
    Q_OBJECT
public:
    EncryptionTests();
private:
    QByteArray message;
private Q_SLOTS:
    void RSAsamePublicKeysAndSameEncryptionFromOnePrivateKey();
    void RSAencryptedMessageDiffersMessage();
    void RSArecoveredMessageEqualMessage();
    void RSAdifferentEncryptedMessagesFromDifferentKeys();
    void RSAdifferentSignaturesFromDifferentKeys();
    void RSAsameSignaturesFromSameKeyAndMessage();
    void RSAdifferentSignatiresFromDifferentMessages();
    void RSAverifySignature();
    void RSAverifySignatureWithOtherKey();
    void RSAsaveLoadPrivateKey();
    void RSAsaveLoadPublicKey();

    void AESencryptionWithDiffKeysDiffer();
    void AESencryptionWithDiffIvDiffer();
    void AESencryptionWithSameKeyIvAreSame();
    void AESrecoveredMessageIsEqualToOrigin();
    void AESencryptionSameMessageTwiceDiffer();

    void DHsharedSecretsAreEqualForPair();
    void DHsharedSecretsOfTwoPairsDIffer();

    void SHAhash();
    void SHAdifferentHashesOfDiffMessages();
    void SHAsameHashOnSameMessage();
    void SHAhmac();
    void SHAdifferentHmacOfDiffMEssages();
    void SHAdifferentHmacOfDiffKeys();
    void SHAsameHmacOnSameMessageAndKey();

};

EncryptionTests::EncryptionTests()
{
    char str[] = "mama mila ramu";
    QByteArray buf(str,sizeof(str));
    message=buf;
}

void EncryptionTests::RSAsamePublicKeysAndSameEncryptionFromOnePrivateKey()
{
    rsacryptor A;
    A.getNewRandomPrivateKey();
    A.getPublicKeyFromPrivate();
    QByteArray cipherMessage1 = A.encrypt(message);
    A.getPublicKeyFromPrivate();
    QByteArray cipherMessage2 = A.encrypt(message);
    bool result = cipherMessage1 == cipherMessage2;
    QVERIFY2(result, "diff keys");
}

void EncryptionTests::RSAencryptedMessageDiffersMessage()
{
    rsacryptor A;
    A.getNewRandomPrivateKey();
    A.getPublicKeyFromPrivate();
    QByteArray cipherMessage1 = A.encrypt(message);
    bool res = message != cipherMessage1;
    QVERIFY2(res, "encrypted message == message");
}

void EncryptionTests::RSArecoveredMessageEqualMessage()
{
    rsacryptor A;
    A.getNewRandomPrivateKey();
    A.getPublicKeyFromPrivate();
    QByteArray cipherMessage1 = A.encrypt(message);
    QByteArray recoveredMessage1 = A.decrypt(cipherMessage1);
    bool res = recoveredMessage1 == message;
    QVERIFY2(res, "recovered message != message");
}

void EncryptionTests::RSAdifferentEncryptedMessagesFromDifferentKeys()
{
    rsacryptor A,B;
    A.getNewRandomPrivateKey();
    A.getPublicKeyFromPrivate();
    B.getNewRandomPrivateKey();
    B.getPublicKeyFromPrivate();
    QByteArray cipherMessage1 = A.encrypt(message);
    QByteArray cipherMessage2 = B.encrypt(message);
    bool result = cipherMessage1 != cipherMessage2;
    QVERIFY2(result, "encrypted messages from diff keys are equal");
}

void EncryptionTests::RSAdifferentSignaturesFromDifferentKeys()
{
    rsacryptor A,B;
    A.getNewRandomPrivateKey();
    B.getNewRandomPrivateKey();
    QByteArray sign1 = A.signMessage(message);
    QByteArray sign2 = B.signMessage(message);
    bool res = sign1 != sign2;
    QVERIFY2(res, "signs from diff keys are equal");
}

void EncryptionTests::RSAsameSignaturesFromSameKeyAndMessage()
{
    rsacryptor A;
    A.getNewRandomPrivateKey();
    QByteArray sign1 = A.signMessage(message);
    QByteArray sign2 = A.signMessage(message);
    bool res = sign1 == sign2;
    QVERIFY2(res, "signs from same key are not equal");
}

void EncryptionTests::RSAdifferentSignatiresFromDifferentMessages()
{
    rsacryptor A;
    A.getNewRandomPrivateKey();
    char str[] = "second message";
    QByteArray message2(str,sizeof(str));
    QByteArray sign1 = A.signMessage(message);
    QByteArray sign2 = A.signMessage(message2);
    bool res = sign1 != sign2;
    QVERIFY2(res, "signs from same key and diff messages are equal");
}

void EncryptionTests::RSAverifySignature()
{
    rsacryptor A;
    A.getNewRandomPrivateKey();
    A.getPublicKeyFromPrivate();
    QByteArray sign1 = A.signMessage(message);
    bool res = A.verifyMessage(message,sign1);
    QVERIFY2(res, "verifier returned false, but must true");
}

void EncryptionTests::RSAverifySignatureWithOtherKey()
{
    rsacryptor A,B;
    A.getNewRandomPrivateKey();
    A.getPublicKeyFromPrivate();
    B.getNewRandomPrivateKey();
    B.getPublicKeyFromPrivate();
    QByteArray sign1 = A.signMessage(message);
    bool res = B.verifyMessage(message,sign1);
    QVERIFY2(!res, "verifier returned true with false key");
}

void EncryptionTests::RSAsaveLoadPrivateKey()
{
    std::string filename = "G:\\privKey";
    rsacryptor A;
    A.getNewRandomPrivateKey();
    A.getPublicKeyFromPrivate();
    QByteArray cm1 = A.encrypt(message);
    A.savePrivateKeyToFile(filename);
    rsacryptor B;
    B.loadPrivateKeyFromFile(filename);
    QByteArray rm1 = B.decrypt(cm1);
    bool res = rm1 == message;
    QVERIFY2(res, "load key isn't same origin key");
}

void EncryptionTests::RSAsaveLoadPublicKey()
{
    std::string filename = "G:\\pubKey";
    rsacryptor A;
    A.getNewRandomPrivateKey();
    A.getPublicKeyFromPrivate();
    QByteArray cm1 = A.encrypt(message);
    A.savePublicKeyToFile(filename);
    rsacryptor B;
    B.loadPublicKeyFromFile(filename);
    QByteArray cm2 = B.encrypt(message);
    bool res = cm1 == cm2;
    QVERIFY2(res, "load key isn't same origin key");
}

void EncryptionTests::AESencryptionWithDiffKeysDiffer()
{
    QByteArray key1(32,0x0A);
    QByteArray key2(32,0xA0);
    QByteArray iv1(16,0x10);
    QByteArray iv2(16,0x7F);
    aescryptor A(key1,iv1),B(key2,iv1);
    QByteArray cm1 = A.encrypt(message);
    QByteArray cm2 = B.encrypt(message);
    bool res = cm1 != cm2;
    QVERIFY2(res,"cipherMessages from different keys are equal");
}

void EncryptionTests::AESencryptionWithDiffIvDiffer()
{
    QByteArray key1(32,0x0A);
    QByteArray key2(32,0xA0);
    QByteArray iv1(16,0x10);
    QByteArray iv2(16,0x7F);
    aescryptor A(key1,iv1),B(key1,iv2);
    QByteArray cm1 = A.encrypt(message);
    QByteArray cm2 = B.encrypt(message);
    bool res = cm1 != cm2;
    QVERIFY2(res,"cipherMessages from different ivs are equal");
}

void EncryptionTests::AESencryptionWithSameKeyIvAreSame()
{
    QByteArray key1(32,0x0A);
    QByteArray key2(32,0xA0);
    QByteArray iv1(16,0x10);
    QByteArray iv2(16,0x7F);
    aescryptor A(key1,iv1),B(key1,iv1);
    QByteArray cm1 = A.encrypt(message);
    QByteArray cm2 = B.encrypt(message);
    bool res = cm1 == cm2;
    QVERIFY2(res,"cipherMessages from one key and iv differ");
}

void EncryptionTests::AESrecoveredMessageIsEqualToOrigin()
{
    QByteArray message("коммандер же : 3 [14:50:43] Виталий !ZiP!: одной хватит [14:53:19] Дмитрий: надо бы скрайленды на замену вратам [14:53:25] Дмитрий: но они сука такие дорогие пздц [14:53:28] Дмитрий: по 10 баксов");
    QByteArray key1(32,0x0A);
    QByteArray iv1(16,0x10);
    aescryptor A(key1,iv1);
    QByteArray cm1 = A.encrypt(message);
    QByteArray rm1 = A.decrypt(cm1);
    bool res = rm1 == message;
    QVERIFY2(res,"recovered message != origin message");
}

void EncryptionTests::AESencryptionSameMessageTwiceDiffer()
{
    QByteArray key1(32,0x0A);
    QByteArray iv1(16,0x10);
    aescryptor A(key1,iv1);
    QByteArray cm1 = A.encrypt(message);
    QByteArray cm2 = A.encrypt(message);
    bool res = cm1 != cm2;
    QVERIFY2(res,"Same message encrypted twice in a row return 1 output");
}

void EncryptionTests::DHsharedSecretsAreEqualForPair()
{
    dhcryptor::initialize();
    QByteArray privKey1;
    QByteArray privKey2;
    QByteArray pubKey1;
    QByteArray pubKey2;
    dhcryptor::getKeyPair(privKey1,pubKey1);
    dhcryptor::getKeyPair(privKey2,pubKey2);
    QByteArray sharedSecret1 = dhcryptor::getSharedSecret(privKey1,pubKey2);
    QByteArray sharedSecret2 = dhcryptor::getSharedSecret(privKey2,pubKey1);
    bool res = sharedSecret1 == sharedSecret2;
    QVERIFY2(res,"Shared secrets are not equal");
}

void EncryptionTests::DHsharedSecretsOfTwoPairsDIffer()
{
    dhcryptor::initialize();
    QByteArray privKey1;
    QByteArray privKey2;
    QByteArray privKey3;
    QByteArray pubKey1;
    QByteArray pubKey2;
    QByteArray pubKey3;
    dhcryptor::getKeyPair(privKey1,pubKey1);
    dhcryptor::getKeyPair(privKey2,pubKey2);
    dhcryptor::getKeyPair(privKey3,pubKey3);
    QByteArray sharedSecret1 = dhcryptor::getSharedSecret(privKey1,pubKey2);
    QByteArray sharedSecret2 = dhcryptor::getSharedSecret(privKey3,pubKey2);
    bool res = sharedSecret1 != sharedSecret2;
    QVERIFY2(res,"Shared secrets from diff keys are equal");
}

void EncryptionTests::SHAhash()
{
    QByteArray hash=shahasher::hash(message);
    bool res = !hash.isEmpty();
    QVERIFY2(res,"hash is empty");
}

void EncryptionTests::SHAhmac()
{
    QByteArray key(32,0xA);
    QByteArray hmac=shahasher::hmac(key,message);
    bool res = !hmac.isEmpty();
    QVERIFY2(res,"hmac is empty");
}

void EncryptionTests::SHAdifferentHashesOfDiffMessages()
{
    QByteArray m1(32,0xA0);
    QByteArray m2(32,0x0B);
    QByteArray hash1 = shahasher::hash(m1);
    QByteArray hash2 = shahasher::hash(m2);
    bool res = hash1 != hash2;
    QVERIFY2(res,"same hash of diff messages");
}


void EncryptionTests::SHAsameHashOnSameMessage()
{
    QByteArray hash1 = shahasher::hash(message);
    QByteArray hash2 = shahasher::hash(message);
    bool res = hash1 == hash2;
    QVERIFY2(res,"diff hash of same messages");
}

void EncryptionTests::SHAdifferentHmacOfDiffMEssages()
{
    QByteArray key(32,0xA);
    QByteArray m1(32,0xA0);
    QByteArray m2(32,0x0B);
    QByteArray hmac1=shahasher::hmac(key,m1);
    QByteArray hmac2=shahasher::hmac(key,m2);
    bool res = hmac1 != hmac2;
    QVERIFY2(res,"same hmac of diff messages");
}

void EncryptionTests::SHAdifferentHmacOfDiffKeys()
{
    QByteArray key1(32,0xA0);
    QByteArray key2(32,0x0B);
    QByteArray hmac1=shahasher::hmac(key1,message);
    QByteArray hmac2=shahasher::hmac(key2,message);
    bool res = hmac1 != hmac2;
    QVERIFY2(res,"same hmac of diff keys");
}

void EncryptionTests::SHAsameHmacOnSameMessageAndKey()
{
    QByteArray key(32,0xA0);
    QByteArray hmac1=shahasher::hmac(key,message);
    QByteArray hmac2=shahasher::hmac(key,message);
    bool res = hmac1 == hmac2;
    QVERIFY2(res,"diff hmac on same keys and message");
}

QTEST_APPLESS_MAIN(EncryptionTests)

#include "tst_encryptiontests.moc"
