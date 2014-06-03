#ifndef MANAGER_H
#define MANAGER_H

#include "connection.h"
#include <QTcpServer>
#include <QObject>
#include <QSet>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = 0);
    void setMaxConnections(quint16 maxConnections);
    bool listen(const QHostAddress & address = QHostAddress::Any, quint16 port = 0)
    {
        return listener.listen(address, port);
    }
private:
    QHostAddress address;
    quint16 port;
    quint16 maxConnections;
    QTcpServer listener;
    QSet<Connection*> openConnections;
signals:

public slots:
private slots:
    void newConnection();
    void toDelete(Connection *connection);
};

#endif // MANAGER_H
