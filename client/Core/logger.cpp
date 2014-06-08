#include "logger.h"

logger::logger(QObject *parent) :
    QObject(parent)
{
}

 void logger::operator << (const QString& message)
 {
     std::cout << message.toStdString() << std::endl;
 }
