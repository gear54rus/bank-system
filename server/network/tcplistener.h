#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <QTcpServer>

class TcpListener : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpListener(QObject *parent = 0);

signals:

public slots:

};

#endif // TCPLISTENER_H
