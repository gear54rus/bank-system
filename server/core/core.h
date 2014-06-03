#ifndef CORE_H
#define CORE_H

#include "../database/db.h"
#include "../network/manager.h"
#include <QObject>
#include <QCoreApplication>

class Core : public QCoreApplication
{
    Q_OBJECT
public:
    Core(int argc, char** argv);

public slots:
    void init();
    void stop();
private:
    DBController database;
    NetworkManager* manager;
private slots:

};

#endif // CORE_H
