#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <QTcpServer>
#include <QTcpSocket>

class TcpListener : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpListener(QObject *parent = 0);
protected:
    virtual void incomingConnection(qintptr handle);
signals:

public slots:

};

#endif // TCPLISTENER_H
