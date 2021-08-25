#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connected = false;
    socket = nullptr;
    connect(&server, SIGNAL(newConnection()), this, SLOT(server_newConnection()));
    ui->btnConnect->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::socket_events()
{
    connect(socket, SIGNAL(connected()), this, SLOT(socket_connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socket_disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socket_error()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socket_readyRead()));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socket_stateChanged()));
}

void MainWindow::on_btnConnect_clicked()
{
    if(!connected)
    {
        update(true);
        socket = new QTcpSocket(this);
        socket_events();
        socket->connectToHost(ui->txtIP->text(), 9999);
        ui->txtReceive->appendPlainText("enter a name:");
    }
    else
    {
        update(false);
        server.close();
        if(socket != nullptr)
        {
            socket->disconnectFromHost();
            socket->deleteLater();
            socket = nullptr;
        }
    }
}



void MainWindow::update(bool connected)
{
    this->connected = connected;
    if(connected)
    {
        ui->btnConnect->setText("disconnect");
        ui->txtIP->setEnabled(false);
        ui->btnSend->setEnabled(true);
    }
    else
    {
        ui->btnConnect->setText("connect");
        ui->txtIP->setEnabled(true);
        ui->btnSend->setEnabled(false);
    }
}

void MainWindow::on_btnSend_clicked()
{
    socket->write(ui->txtSend->toPlainText().toUtf8());
}

void MainWindow::on_btnLed1On_clicked()
{
    socket->write("ardu led 1 on");
}

void MainWindow::on_btnLed1Off_clicked()
{
    socket->write("ardu led 1 off");
}

void MainWindow::on_btnLed2On_clicked()
{
    socket->write("ardu led 2 on");
}

void MainWindow::on_btnLed2Off_clicked()
{
    socket->write("ardu led 2 off");
}

void MainWindow::on_btnBuzzerOn_clicked()
{
    socket->write("ardu buzzer on");
}

void MainWindow::on_btnLCD_clicked()
{
    ui->btnSend->setEnabled(true);
    ui->txtSend->clear();
    ui->txtSend->appendPlainText("ardu lcd ");
    ui->txtReceive->appendPlainText("please enter your message after <ardu lcd ...>\n");
}

void MainWindow::on_btnClient_clicked()
{
    ui->btnSend->setEnabled(true);
    ui->txtSend->clear();
    ui->txtSend->appendPlainText("clnt ");
    ui->txtReceive->appendPlainText("please enter reciever name and your message after <clnt ...>\n");
}

void MainWindow::on_btnList_clicked()
{
    socket->write("list");
}
void MainWindow::on_btnClose_clicked()
{
    socket->write("exit");
}

void MainWindow::socket_connected()
{
    log("socket_connected");
}

void MainWindow::socket_disconnected()
{
    log("socket_disconnected");
    update(false);
}

void MainWindow::socket_error()
{
    log("socket_error [" + QString::number(socket->error()) + "] : " + socket->errorString());
}

void MainWindow::socket_readyRead()
{
    while(socket->bytesAvailable())
    {
        QByteArray data = socket->readAll();
        log(data);
    }
}

void MainWindow::server_newConnection()
{
    log("server_newConnection");
    socket = server.nextPendingConnection();
    socket_events();
    server.close();
}

void MainWindow::log(QString msg)
{
    ui->txtReceive->appendPlainText(msg + "\n---");
}

