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
    Connection* newConnection = new Connection(listener.nextPendingConnection());
    connect(newConnection, SIGNAL(disconnected()), this, SLOT(toDelete()), Qt::QueuedConnection);
    openConnections.insert(newConnection);
    if (openConnections.size() < maxConnections)
    {
        listener.resumeAccepting();
    }
}

void NetworkManager::toDelete()
{
    Connection* sender = dynamic_cast<Connection*>(QObject::sender());
    openConnections.remove(sender);
    sender->deleteLater();
    if (openConnections.size() < maxConnections)
    {
        listener.resumeAccepting();
    }
}
