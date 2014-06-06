#include "secbytearray.h"


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
        quint64 diff = this->size() - size,
                cSize = this->size();
        char* data = this->data();
        while (diff--)
        {
            data[cSize - diff] = 0;
        }
    }
    QByteArray::resize(size);
}

