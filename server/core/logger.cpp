#include <iostream>
#include "logger.h"
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>

namespace
{
QMutex MainLock;
LOG_TYPE Level = Log_Message;
QString LevelNames[] = {"DEBUG", "MSG", "ERROR", "CRITICAL"};
}

void SetLogLevel(LOG_TYPE logLevel)
{
    Level = logLevel;
}

void Log(const QString& message, const QString& component, LOG_TYPE type)
{
    if (type >= Level)
    {
        QMutexLocker Lock(&MainLock);
        std::cout << QString("%1 [%2|%3] %4").arg(QDateTime::currentDateTime().toString("dd.MM.yy HH:mm:ss:zzz"), component, LevelNames[static_cast<int>(type) + 1], message).toStdString() << std::endl;
    }
}
