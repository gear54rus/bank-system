#include "core.h"
#include "logger.h"
#include <QTimer>
#include <QSettings>

Core::Core(int argc, char **argv):
    QCoreApplication(argc, argv)
{
    QTimer::singleShot(0, this, SLOT(init()));
}

void Core::init()
{
    try
    {
        QSettings s("settings.ini", QSettings::IniFormat);
        s.beginGroup("Database");
        if (!db.connect(s.value("host", "none").toString(),
                        s.value("port", 3306).toUInt(),
                        s.value("database", "none").toString(),
                        s.value("user", "none").toString(),
                        s.value("password", "none").toString()))
        {
            throw 1;
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
