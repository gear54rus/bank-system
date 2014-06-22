#include "db.h"
#include "../core/logger.h"
#include <cryptopp/dll.h>
#include <QSqlError>
#include <QTimer>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QSqlRecord>

DBController::DBController(QObject* parent) :
    QObject(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
}

bool DBController::connect(const QString& host, quint16 port, const QString& dbname, const QString& username, const QString& password)
{
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(password);
    if(db.open()) {
        return true;
    } else {
        Log(db.lastError().databaseText(), "Database", Log_Critical);
        return false;
    }
}
void DBController::addConnection(Connection* target)
{
    Session* s = new Session(target);
    sessions.insert(s);
    QObject::connect(target, SIGNAL(disconnected()), this, SLOT(toDelete()));
    QObject::connect(target, SIGNAL(received(SecByteArray*)), this, SLOT(processData(SecByteArray*)));
    s->send(new SecByteArray(QString("login:").toUtf8()));
}

void DBController::processData(SecByteArray* data)
{
    Connection* sender = dynamic_cast<Connection*>(QObject::sender());
    Session* s = 0;
    foreach(Session * cur, sessions) {
        if(cur->getConnection() == sender) {
            s = cur;
            break;
        }
    }
    if(s) {
        QString message = QString(*data);
        QStringList parts = message.split(' ');
        try {
            if(message == "disconnect;") {
                Log(QString("%1 has requested to end session. Session ended OK!").arg(s->getConnection()->getRemote()), "Database");
                s->end(true);
                delete data;
                return;
            }
            if(parts.length() < 3 || message[message.length() - 1] != ';') {
                throw 1;
            }
            if(s->loggedIn) {
                if(parts[0] == "balance" && parts[1] == "alter") {
                    bool ok;
                    QRegularExpressionMatch match = QRegularExpression("^balance alter ([\\+\\-]?\\d{0,18});$").match(message);
                    qlonglong amount = match.captured(1).toLongLong(&ok);
                    if(match.hasMatch() && ok) {
                        QSqlQuery q = db.exec(QString("SELECT `balance` FROM `users` WHERE `name` = '%1';").arg(s->login));
                        q.next();
                        qlonglong balance = q.value(0).toString().toLongLong();
                        if(!amount) {
                            s->send(new SecByteArray(QString("code 0 %1;").arg(QSN(balance)).toUtf8()));
                            Log(QString("Balance request from %1: balance is %2.").arg(s->getConnection()->getRemote(), QSN(balance)), "Database", Log_Debug);
                        } else {
                            if(balance > 0 || amount > 0) {
                                db.exec(QString("UPDATE `users` SET `balance` = %1 WHERE `name` = '%2';").arg(QSN(balance + amount), s->login));
                                s->send(new SecByteArray(QString("code 0 %1;").arg(QSN(balance + amount)).toUtf8()));
                                Log(QString("Balance alteration request from %1: altered by %2 and is now %3.").arg(s->getConnection()->getRemote(), QSN(amount), QSN(balance + amount)), "Database", Log_Debug);
                            } else {
                                Log(QString("Balance alteration request from %1: not enough funds!").arg(s->getConnection()->getRemote()), "Database", Log_Debug);
                                s->send(new SecByteArray(QString("code 1 not enough funds;").toUtf8()));
                            }
                        }
                    } else {
                        Log(QString("Command from %1: unable to parse alter amount!").arg(s->getConnection()->getRemote()), "Database", Log_Debug);
                        s->send(new SecByteArray(QString("code 1 unable to parse amount;").toUtf8()));
                    }
                    s->send(new SecByteArray(QString("command:").toUtf8()));
                } else {
                    Log(QString("Command from %1: expected \"balance alter\", got \"%2\"!").arg(s->getConnection()->getRemote(), parts[0] + " " + parts[1]), "Database", Log_Debug);
                    throw 1;
                }
            } else {
                if(parts[0] == "login") {
                    QRegularExpressionMatch match = QRegularExpression("^login ([\\da-zA-Z]{4,32}) (.{6,128});$").match(message);
                    if(match.hasMatch()) {
                        QSqlQuery q = db.exec(QString("SELECT `password` FROM `users` WHERE `name` = '%1';").arg(match.captured(1)));
                        if(q.next()) {
                            if(q.value(0).toString().toUtf8() == Connection::SHA256(match.captured(2).toUtf8()).toHex()) {
                                s->loggedIn = true;
                                s->login = match.captured(1);
                                Log(QString("Login attempt from %1: logged in as \"%2\"!").arg(s->getConnection()->getRemote(), s->login), "Database", Log_Error);
                                s->send(new SecByteArray(QString("code 0;").toUtf8()));
                                s->send(new SecByteArray(QString("command:").toUtf8()));
                            }
                        } else {
                            s->send(new SecByteArray(QString("code 1 invalid username or password;").toUtf8()));
                            Log(QString("Login attempt from %1: invalid username or password!").arg(s->getConnection()->getRemote()), "Database", Log_Error);
                            throw 1;
                        }
                    } else {
                        Log(QString("Malformed login message from %1!").arg(s->getConnection()->getRemote()), "Database", Log_Error);
                        throw 1;
                    }
                } else {
                    Log(QString("Command from %1: expected \"login\", got \"%2\"!").arg(s->getConnection()->getRemote(), parts[0]), "Database", Log_Debug);
                    throw 1;
                }
            }
        } catch(int) {
            s->end(false);
            Log(QString("Bad command received from %1. Session ended abnormally!").arg(s->getConnection()->getRemote()), "Database", Log_Error);
        }
    }
    delete data;
}

void DBController::toDelete(Connection* target)
{
    if(!target) {
        target = dynamic_cast<Connection*>(QObject::sender());
    }
    foreach(Session * cur, sessions) {
        if(cur->getConnection() == target) {
            sessions.remove(cur);
            cur->deleteLater();
            break;
        }
    }
}
