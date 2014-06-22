#include "core.h"
#include "logger.h"
#include "../config.h"
#include "../global.h"
#include <QTimer>
#include <QThread>
#include <QSettings>

Core::Core(int argc, char** argv):
    QCoreApplication(argc, argv),
    database(this)
{
    Log("Starting...", "Core");
    init();
}

void Core::init()
{
    Log("Initializing...", "Core");
    try {
        QSettings s("settings.ini", QSettings::IniFormat);
        s.beginGroup("Log");
        {
            qint8 level = s.value("level", 0).toInt();
            SetLogLevel(static_cast<LOG_TYPE>(level));
            Log(QString("Log level set to \"%1 and higher\".").arg(LevelNames[level + 1]), "Core");
        }
        s.endGroup();
        s.beginGroup("Database");
        {
            QString host = s.value("host", "none").toString(),
                    db = s.value("database", "none").toString(),
                    user = s.value("user", "none").toString(),
                    pw = s.value("password", "none").toString();
            quint16 port = s.value("port", CORE_MYSQL_DEFAULT_PORT).toUInt();
            Log(QString("Connecting to MySQL database \"%1\" on \"%2:%3\" as \"%4\"...").arg(db, host, QSN(port), user), "MySQL");
            if(!database.connect(host, port, db, user, pw)) {
                throw 1;
            }
            Log("Successfully connected to database!", "MySQL");
        }
        s.endGroup();
        s.beginGroup("Network");
        {
            QHostAddress host = QHostAddress(s.value("bind_ip", "0.0.0.0").toString());
            host = (!s.value("bind_ip", "0.0.0.0").toString().compare("localhost", Qt::CaseInsensitive)) ? QHostAddress(QHostAddress::LocalHost) : ((host.isNull()) ? QHostAddress::AnyIPv4 : host);
            quint16 port = s.value("bind_port", CORE_DEFAULT_PORT).toUInt();
            quint8 maxConnections = s.value("max_connections", "5").toUInt();
            manager = new NetworkManager();
            connect(manager, SIGNAL(newSecureConnection(Connection*)), &database, SLOT(addConnection(Connection*)));
            manager->setMaxConnections(maxConnections);
            Log(QString("Maximum simultaneous connections set to %1.").arg(QSN(maxConnections)), "Network");
            Log(QString("Attempting to bind to \"%1:%2\"...").arg(host.toString(), QSN(port)), "Network");
            if(manager->listen(host, port)) {
                Log(QString("Now listening on \"%1:%2\"!").arg(host.toString(), QSN(port)), "Network");
            } else {
                Log(QString("Failed to bind to \"%1:%2\"!").arg(host.toString(), QSN(port)), "Network", Log_Critical);
                throw 1;
            }
            manager->moveToThread(new QThread(this));
            manager->thread()->start();
        }
        s.endGroup();
        s.beginGroup("RSA");
        {
            SecByteArray key = SecByteArray::fromBase64(s.value("private_key", "").toByteArray());
            if(!key.size()) {
                Connection::init();
                s.setValue("private_key", QString(Connection::getRSAPrivate().toBase64()));
                Log("New RSA keypair was generated - check \"settings.ini\".", "Core", Log_Critical);
            } else {
                Connection::init(key);
                Log("Using existing keypair...", "Core", Log_Message);
            }
            s.setValue("public_key", QString(Connection::getRSAPublic().toBase64()));
            s.sync();
        }
    } catch(int) {
        QTimer::singleShot(0, this, SLOT(stop()));
        return;
    }
    Log("Initialization finished!", "Core");
}
void Core::stop()
{
    Log("Fatal error encountered! Exiting...", "Core", Log_Critical);
    this->exit(1);
}
