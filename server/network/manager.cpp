#include "manager.h"

NetworkManager::NetworkManager(QObject *parent, quint16 maxConnections) :
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
    openConnections.insert(listener.nextPendingConnection());
    int a = openConnections.size();
    if (openConnections.size() < maxConnections)
    {
        listener.resumeAccepting();
    }
}

void NetworkManager::toDelete(QTcpSocket* connection)
{
    openConnections.remove(connection);
    connection->disconnectFromHost();
    connection->waitForDisconnected(5000);
    connection->deleteLater();
    if (openConnections.size() < maxConnections)
    {
        listener.resumeAccepting();
    }
}
