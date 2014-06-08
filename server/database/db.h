#ifndef DB_H
#define DB_H

#include <QObject>
#include <QSqlDatabase>

class DBController : public QObject {
    Q_OBJECT
public:
    explicit DBController(QObject* parent = 0);
    bool connect(const QString& host, quint16 port, const QString& dbname, const QString& username, const QString& password);
private:
    QSqlDatabase db;

signals:

public slots:

};

#endif // DB_H
