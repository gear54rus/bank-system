#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QCoreApplication>
#include "../database/db.h"
#include "../network/listener.h"

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
    TcpListener listener;
private slots:

};

#endif // CORE_H
