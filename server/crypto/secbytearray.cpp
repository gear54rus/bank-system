#include "secbytearray.h"

SecByteArray::SecByteArray():
    QByteArray()
{
}

SecByteArray::~SecByteArray()
{
    fill(0);
}

void SecByteArray::resize(int size)
{
    if (size < 0)
        size = 0;
    if (size < this->size())
    {
        size_t diff = this->size() - size,
               cSize = this->size();
        char* data = this->data();
        while (diff--)
        {
            data[cSize - diff] = 0;
        }
    }
    QByteArray::resize(size);
}

