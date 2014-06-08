#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <iostream>

class logger : public QObject
{
    Q_OBJECT
public:
    explicit logger(QObject *parent = 0);
    void operator << (const QString& message);
};

#endif // LOGGER_H
