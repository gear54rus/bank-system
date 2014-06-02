#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <windows.h>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);

signals:
    void sendNumber(char num);
public slots:
    void newConnection();
    void readyRead();
private slots:
    void numberSender(char num);
private:
    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // MYTCPSERVER_H
