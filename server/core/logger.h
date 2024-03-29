#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QList>
#include <QStringList>

enum LOG_TYPE {Log_Debug = -1, Log_Message, Log_Error, Log_Critical};
const QString LevelNames[] = {"DEBUG", "MSG", "ERROR", "CRITICAL"};

void SetLogLevel(LOG_TYPE logLevel);

void Log(const QString& message, const QString& component = "General", LOG_TYPE type = Log_Message);
void Log(const QStringList& message, QList<LOG_TYPE> types);

#endif // LOGGER_H
