#ifndef DB_H
#define DB_H

#include "../network/connection.h"
#include "../crypto/secbytearray.h"
#include "session.h"
#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QMap>

class DBController : public QObject {
    Q_OBJECT
public:
    explicit DBController(QObject* parent = 0);
    bool connect(const QString& host, quint16 port, const QString& dbname, const QString& username, const QString& password);
private:
    QSqlDatabase db;
    QSet<Session*> sessions;
signals:

public slots:
    void addConnection(Connection* target);
    void processData(SecByteArray* data);
    void toDelete(Connection* target = 0);
};

#endif // DB_H
