#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QThread>


#include <../Core/connection.h>

namespace Ui {
class MainWindow;
}

namespace ClientStates
{
    enum ClientState
    {
        DISCONNECTED, CONNECTING, SECURE_CONNECTION, LOGINING, LOGINED
    };
}

using namespace ClientStates;

Q_DECLARE_METATYPE( ClientStates::ClientState );

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void setRsaPublicKeyPath(QString rsaPublicKeyPath);
    void setAddressAndPort(QString address, quint16 port);
    void setConnection();
    bool sendData(const QByteArray message);
    void closeConnection(QString reason, bool isByServer = false);
    QByteArray getLastReceivedMessage();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void gotNewMessage(QByteArray message);
    void newLogMessage(logMessageType logLevel, QString logMessage);
    void connectionSecureStateChanged(bool isSecure);

    void on_bOpenPubkeyFile_clicked();
    void on_pClose_clicked();
    void on_bLogin_clicked();

    void on_bAdd_clicked();

    void on_bSub_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    ClientState _clientState;
    void changeClientState(ClientState newState);
    QThread* _connectionThread;
    connection* _connection;

    void setConnectionSettingsEnabled(bool enable);
    void setAccountActionsEnabled(bool enable);
    void setLoginSettingsEnabled(bool enable);
};

#endif // MAINWINDOW_H
