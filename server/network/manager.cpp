#include "manager.h"

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent),
    listener(this)
{
    listener.setMaxPendingConnections(1);
    connect(&listener, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void NetworkManager::setMaxConnections(quint16 maxConnections)
{
    this->maxConnections = maxConnections;
}

void NetworkManager::newConnection()
{
    listener.pauseAccepting();
    openConnections.insert(new Connection(listener.nextPendingConnection()));
    if (openConnections.size() < maxConnections)
    {
        listener.resumeAccepting();
    }
}

void NetworkManager::toDelete(Connection* connection)
{
    openConnections.remove(connection);
    connection->deleteLater();
    if (openConnections.size() < maxConnections)
    {
        listener.resumeAccepting();
    }
}
