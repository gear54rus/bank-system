#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace ClientStates;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->cbFilter->addItem("DEBUG");
    ui->cbFilter->addItem("INFO");
    ui->cbFilter->addItem("WARNING");
    ui->cbFilter->addItem("ERROR");

    changeClientState(ClientStates::DISCONNECTED);

    //пробуем другой тред, пух.
    _connectionThread = new QThread();
    _connection = new connection();

    qRegisterMetaType<logMessageType>("logMessageType");

    connect(this,SIGNAL(closeConnection(QString,bool)),_connection, SLOT(closeConnection(QString,bool)));
    connect(this,SIGNAL(setConnection()),_connection, SLOT(setConnection()));
    connect(this,SIGNAL(sendData(QByteArray)),_connection, SLOT(sendData(QByteArray)));
    connect(this,SIGNAL(setRsaPublicKeyPath(QString)),_connection, SLOT(setRsaPublicKeyPath(QString)));
    connect(this,SIGNAL(setAddressAndPort(QString,quint16)),_connection, SLOT(setAddressAndPort(QString, quint16)));
    connect(_connection,SIGNAL(connecionChangeSecureState(bool)),this,SLOT(connectionSecureStateChanged(bool)));
    connect(_connection,SIGNAL(gotNewMessage(QByteArray)),this,SLOT(gotNewMessage(QByteArray)));
    connect(_connection,SIGNAL(newLogMessage(logMessageType,QString)),this,SLOT(newLogMessage(logMessageType,QString)));

    _connection->moveToThread(_connectionThread);
    _connectionThread->start();

    //to delete
    this->ui->ePort->setText("8815");
    this->ui->eAddress->setText("192.168.0.31");
    this->ui->ePubkeyPath->setText("C:\\Users\\Crazy_000\\Documents\\Repositories\\bank-system\\client\\build-ClientApplication-Desktop_Qt_5_2_0_MinGW_32bit-Debug\\pubKey.ini");

}

MainWindow::~MainWindow()
{
    _connectionThread->quit();
    _connectionThread->deleteLater();
    _connection->deleteLater();
}

void MainWindow::on_bOpenPubkeyFile_clicked()
{
    QString filename =  QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    this->ui->ePubkeyPath->setText(filename);
}

void MainWindow::on_pClose_clicked()
{
    this->close();
}

void MainWindow::gotNewMessage(QByteArray message)
{
    switch (_clientState)
    {
    case ClientStates::DISCONNECTED:
        break;
    case ClientStates::CONNECTING:
        break;
    case ClientStates::SECURE_CONNECTION:
        break;
    case ClientStates::LOGINING:
    {
        QString str(message);
        if (str.indexOf(QString("success")))
        {
            changeClientState(ClientStates::LOGINED);
            QByteArray message;
            message.append("balance get;");
            emit(sendData(message));
        }
        else
        {
            newLogMessage(ERROR,"login failed: " + str);
            changeClientState(ClientStates::SECURE_CONNECTION);
        }

    }
        break;
    case ClientStates::LOGINED:
    {
        //DELETE IT
        newLogMessage(DEBUG, message);
    }
        break;
    //end of switch
    }
}

void MainWindow::newLogMessage(logMessageType logLevel, QString logMessage)
{
    if (logMessage == "Socket operation timed out")
    {
        changeClientState(ClientStates::DISCONNECTED);
    }
    if (logMessage.indexOf(QString("disconnected")))
    {
        changeClientState(ClientStates::DISCONNECTED);
    }
    if (logLevel >= ui->cbFilter->currentIndex())
    {
        QString message;
        switch(logLevel)
        {
        case DEBUG:
            message.append("DEBUG: ");
            break;
        case INFO:
            message.append("INFO: ");
            break;
        case WARNING:
            message.append("WARNING: ");
            break;
        case ERROR:
            message.append("ERROR: ");
            break;
        }
        ui->logBrowser->append(message.append(logMessage));
    }
}

void MainWindow::connectionSecureStateChanged(bool isSecure)
{
    isSecure? changeClientState(ClientStates::SECURE_CONNECTION) : changeClientState(ClientStates::DISCONNECTED);
}



void MainWindow::changeClientState(ClientState newState)
{
    _clientState = newState;
    QString status;
    switch (_clientState)
    {
    case ClientStates::DISCONNECTED:
        status = "disconnected";
        ui->bLogin->setText("Set connection");
        ui->bLogin->setEnabled(true);

        setLoginSettingsEnabled(false);

        setConnectionSettingsEnabled(true);

        setAccountActionsEnabled(false);

        ui->lClientStatus->setStyleSheet("QLabel { color : red; }");
        ui->lClientStatus->setText("Client status: disconnected");

        break;
    case ClientStates::CONNECTING:
        status = "connecting";

        ui->bLogin->setEnabled(false);
        ui->bOpenPubkeyFile->setEnabled(false);

        setConnectionSettingsEnabled(false);

        ui->lClientStatus->setText("Client status: connecting");
        ui->lClientStatus->setStyleSheet("QLabel { color : orange; }");

        break;
    case ClientStates::SECURE_CONNECTION:
        status = "connected";

        ui->bLogin->setText("Login");
        ui->bLogin->setEnabled(true);

        setLoginSettingsEnabled(true);

        ui->lClientStatus->setText("Client status: connected");
        ui->lClientStatus->setStyleSheet("QLabel { color : purple; }");

        break;
    case ClientStates::LOGINING:
        status = "logining";

        ui->bLogin->setEnabled("false");

        setLoginSettingsEnabled(false);

        ui->lClientStatus->setText("Client status: logining");
        ui->lClientStatus->setStyleSheet("QLabel { color : purple; }");
        break;
    case ClientStates::LOGINED:
        status = "logined";

        ui->bLogin->setEnabled(true);
        ui->bLogin->setText("Logout");

        setAccountActionsEnabled(true);

        ui->lClientStatus->setText("Client status: logined");
        ui->lClientStatus->setStyleSheet("QLabel { color : green; }");
        break;
    }
    newLogMessage(INFO, "client is " + status);
}

void MainWindow::on_bLogin_clicked()
{
    switch (_clientState)
    {
    case ClientStates::DISCONNECTED:
        changeClientState(CONNECTING);
        emit(setRsaPublicKeyPath(ui->ePubkeyPath->text()));
        emit(setAddressAndPort(ui->eAddress->text(),ui->ePort->text().toUShort()));
        emit(setConnection());
        break;
    case ClientStates::CONNECTING:
        break;
    case ClientStates::SECURE_CONNECTION:
    {
        QByteArray message;
        message.append("login ").append(ui->eLogin->text()).append((" ")).append(ui->ePassword->text()).append(";");
        emit (sendData(message));
        changeClientState(LOGINING);
    }
        break;
    case ClientStates::LOGINED:
        emit(sendData("logout"));
        changeClientState(ClientStates::SECURE_CONNECTION);
        break;
    }

}

void MainWindow::on_bAdd_clicked()
{
    QByteArray message;
    message.append("balance alter +").append(ui->eMoney->text().append(";"));
    emit(sendData(message));
    newLogMessage(DEBUG,"balance alter + is sent: " + message);
}

void MainWindow::on_bSub_clicked()
{
    QByteArray message;
    message.append("balance alter -").append(ui->eMoney->text().append(";"));
    emit(sendData(message));
    newLogMessage(DEBUG,"balance alter - is sent: " + message);
}


void MainWindow::setConnectionSettingsEnabled(bool enable)
{
    ui->eAddress->setReadOnly(!enable);
    ui->ePort->setReadOnly(!enable);
    ui->ePubkeyPath->setReadOnly(!enable);
    ui->bOpenPubkeyFile->setEnabled(enable);
}

void MainWindow::setAccountActionsEnabled(bool enable)
{
    ui->eMoney->setReadOnly(!enable);
    ui->bAdd->setEnabled(enable);
    ui->bSub->setEnabled(enable);
}

void MainWindow::setLoginSettingsEnabled(bool enable)
{
    ui->eLogin->setReadOnly(!enable);
    ui->ePassword->setReadOnly(!enable);
}

void MainWindow::on_pushButton_clicked()
{
    QByteArray mess;
    mess.append(ui->lineEdit->text());
    emit (sendData(mess));
}
