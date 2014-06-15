#ifndef SESSION_H
#define SESSION_H

#include "../crypto/secbytearray.h"
#include "../network/connection.h"
#include <QObject>

class Session : public QObject {
    Q_OBJECT
public:
    explicit Session(Connection* connection, QObject* parent = 0);
    void send(SecByteArray* data);
    inline Connection* getConnection()
    {
        return connection;
    }
    void end(bool normal);
    bool loggedIn;
    QString login;
private:
    Connection* connection;
signals:
    void closeConnection(bool normal);
    void sendData(SecByteArray* data);
public slots:

};

#endif // SESSION_H
