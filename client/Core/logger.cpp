#include "logger.h"

logger::logger(QObject *parent) :
    QObject(parent)
{
    connect(this,SIGNAL(newMessage(QString)),this,SLOT(sendNewMessage(QString)));
}


 void logger::operator << (const QString& message)
 {
     emit(newMessage(message));
 }

 void logger::sendNewMessage(const QString &message)
 {
     std::cout << message.toStdString() << std::endl;
 }
