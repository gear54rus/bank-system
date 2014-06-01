#include "core.h"
#include "logger.h"

Core::Core(int argc, char **argv):
    QCoreApplication(argc, argv)
{
    QSettings s("settings.ini", QSettings::IniFormat);
}
