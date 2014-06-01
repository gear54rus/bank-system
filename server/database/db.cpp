#include "db.h"
#include "../core/logger.h"
#include <QSqlError>

DBController::DBController(QObject *parent) :
    QObject(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
}

bool DBController::connect(const QString& host, quint16 port, const QString& dbname, const QString& username, const QString& password)
{
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(password);
    if (db.open())
    {
        return 1;
    }
    else
    {
        Log(db.lastError().databaseText(), "Database", Log_Critical);
        return 0;
    }
}
