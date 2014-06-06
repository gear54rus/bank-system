#include "clientcontroller.h"

ClientController::ClientController(Client* client, QObject *parent) :
    QObject(parent)
{
    connect(this,SIGNAL(tryClientNextStage()), client, SLOT(tryNextStage()),Qt::DirectConnection);
}


void ClientController::next()
{
    emit(tryClientNextStage());
}
