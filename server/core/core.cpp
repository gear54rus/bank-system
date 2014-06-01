#include "core.h"
#include "logger.h"
#include "../config.h"
#include "../global.h"
#include <QTimer>
#include <QSettings>

Core::Core(int argc, char **argv):
    QCoreApplication(argc, argv)
{
    Log("Starting", "Core");
    QTimer::singleShot(0, this, SLOT(init()));
}

void Core::init()
{
    Log("Initializing...", "Core");
    try
    {
        QSettings s("settings.ini", QSettings::IniFormat);
        s.beginGroup("Log");
        SetLogLevel(static_cast<LOG_TYPE>(s.value("level", 0).toInt()));
        s.endGroup();
        s.beginGroup("Database");
        if (!database.connect(s.value("host", "none").toString(),
                              s.value("port", 3306).toUInt(),
                              s.value("database", "none").toString(),
                              s.value("user", "none").toString(),
                              s.value("password", "none").toString()))
        {
            throw 1;
        }
        s.endGroup();
        s.beginGroup("Network");
        {
            QHostAddress host = QHostAddress(s.value("bind_ip", "0.0.0.0").toString());
            host = (host.isNull()) ? QHostAddress::AnyIPv4 : host;
            quint16 port = s.value("bind_port", CORE_DEFAULT_PORT).toUInt();
            Log(QString("Attempting to bind to \"%1:%2\"...").arg(host.toString(), QSN(port)), "Network");
            if (listener.listen(host, port))
            {
                Log(QString("Successfully bound to \"%1:%2\"!").arg(host.toString(), QSN(port)), "Network");
            }
            else
            {
                Log(QString("Failed to bind to \"%1:%2\"!").arg(host.toString(), QSN(port)), "Network", Log_Critical);
                throw 1;
            }
        }
    }
    catch (int)
    {
        QTimer::singleShot(0, this, SLOT(stop()));
    }
}
void Core::stop()
{
    Log("Fatal error encountered! Exiting...", "Core", Log_Critical);
    this->exit(1);
}
