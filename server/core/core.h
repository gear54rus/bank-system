#ifndef CORE_H
#define CORE_H

#include <QCoreApplication>
#include <QSettings>

class Core : public QCoreApplication
{
public:
    Core(int argc, char** argv);
    void Init();
private:

};

#endif // CORE_H
