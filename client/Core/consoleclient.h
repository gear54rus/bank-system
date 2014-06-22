#ifndef CONSOLECLIENT_H
#define CONSOLECLIENT_H

#include <QObject>

class ConsoleClient : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleClient(QObject *parent = 0);
    void setConnection();
signals:

public slots:

};

#endif // CONSOLECLIENT_H
