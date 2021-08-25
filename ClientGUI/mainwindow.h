#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();
    void on_btnSend_clicked();
    void on_btnLed1On_clicked();
    void on_btnLed1Off_clicked();
    void on_btnLed2On_clicked();
    void on_btnLed2Off_clicked();
    void on_btnBuzzerOn_clicked();
    void on_btnLCD_clicked();
    void on_btnClient_clicked();
    void on_btnList_clicked();
    void on_btnClose_clicked();
    void socket_connected();
    void socket_disconnected();
    void socket_error();
    void socket_readyRead();
    void server_newConnection();

private:
    Ui::MainWindow *ui;
    bool connected;
    QTcpServer server;
    QTcpSocket *socket;

private:
    void log(QString msg);
    void socket_events();
    void update(bool connected);
};

#endif // MAINWINDOW_H
