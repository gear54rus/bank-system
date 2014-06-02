#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <windows.h>

class MyTcpClient : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpClient(QObject *parent = 0);
    void startConnection(QString address, quint16 port);
private slots:
    void numberSender(char num);
    void readyRead();
signals:
    void sendNumber(char num);
public slots:
private:
    QTcpSocket* socket;
};

#endif // MYTCPCLIENT_H
