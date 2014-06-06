#ifndef SECBYTEARRAY_H
#define SECBYTEARRAY_H

#include <QByteArray>

class SecByteArray : public QByteArray
{
public:
    SecByteArray(): QByteArray() {}
    SecByteArray(const char * data, int size = -1): QByteArray(data, size) {}
    SecByteArray(int size, char ch): QByteArray(size, ch) {}
    SecByteArray(const SecByteArray & other): QByteArray(other) {}
    SecByteArray(const QByteArray & other): QByteArray(other) {}
    ~SecByteArray();
    void resize(int size);
};

#endif // SECBYTEARRAY_H
