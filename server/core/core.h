#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QCoreApplication>
#include "../database/db.h"

class Core : public QCoreApplication
{
    Q_OBJECT
public:
    Core(int argc, char** argv);

public slots:
    void init();
    void stop();
private:
    DBController db;
private slots:

};

#endif // CORE_H
