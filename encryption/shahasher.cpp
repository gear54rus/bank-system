#include "shahasher.h"

shaHasher::shaHasher()
{
}


CryptoPP::SecByteBlock shaHasher::hash(byte* data, int length)
{
    CryptoPP::SHA256 hasher;

}
