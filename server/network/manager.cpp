#include "manager.h"

NetworkManager::NetworkManager(QObject* parent) :
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
    connect(newConnection, SIGNAL(secured()), this, SLOT(secured()), Qt::QueuedConnection);
    openConnections.insert(newConnection);
    if(openConnections.size() < maxConnections) {
        listener.resumeAccepting();
    }
}
void NetworkManager::secured()
{
    emit newSecureConnection(dynamic_cast<Connection*>(QObject::sender()));
}

void NetworkManager::toDelete(Connection* target)
{
    if(!target) {
        target = dynamic_cast<Connection*>(QObject::sender());
    }
    openConnections.remove(target);
    target->deleteLater();
}
