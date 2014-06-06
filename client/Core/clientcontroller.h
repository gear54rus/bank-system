#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include <QObject>
#include <client.h>

class ClientController : public QObject
{
    Q_OBJECT
public:
    explicit ClientController(Client* client, QObject *parent = 0);
    void next();
signals:
    void tryClientNextStage();
public slots:

};

#endif // CLIENTCONTROLLER_H
