#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lConnectionState->setStyleSheet("QLabel { color : red; }");
    ui->cbFilter->addItem("DEBUG");
    ui->cbFilter->addItem("INFO");
    ui->cbFilter->addItem("WARNING");
    ui->cbFilter->addItem("ERROR");
    _clientState = UNLOGINED;

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
    //delete _connection;
    _connectionThread->exit();
    delete _connectionThread;
    delete ui;



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
    case UNLOGINED:
        break;
    case LOGINING:
    {
        QString str(message);
        if (str.indexOf(QString("success")))
        {
            changeClientState(LOGINED);
            QByteArray message;
            message.append("balance get;");
            emit(sendData(message));
        }

    }
        break;
    case LOGINED:
        break;
    }
}

void MainWindow::newLogMessage(logMessageType logLevel, QString logMessage)
{
    if (logLevel >= ui->cbFilter->currentIndex())
    {
        ui->logBrowser->append(logMessage);
    }
}

void MainWindow::connectionSecureStateChanged(bool isSecure)
{
    if (isSecure)
    {
        ui->lConnectionState->setStyleSheet("QLabel { color : green; }");
        if (_clientState == UNLOGINED)
        {
            QByteArray message;
            message.append("login ").append(ui->eLogin->text()).append((" ")).append(ui->ePassword->text()).append(";");
            emit (sendData(message));
        }
    }
    else
    {
        ui->lConnectionState->setStyleSheet("QLabel { color : red; }");
    }
}



void MainWindow::changeClientState(ClientState newState)
{
    _clientState = newState;
    QString mess;
    switch (_clientState)
    {
    case UNLOGINED:
        mess ="client is unlogined";
        break;
    case LOGINING:
        mess = "client is logining";
        break;
    case LOGINED:
        mess = "client is logined";
        break;
    }
    newLogMessage(INFO, mess);
}

void MainWindow::on_bLogin_clicked()
{
    emit(setRsaPublicKeyPath(ui->ePubkeyPath->text()));
    emit(setAddressAndPort(ui->eAddress->text(),ui->ePort->text().toUShort()));
    emit(setConnection());
}
