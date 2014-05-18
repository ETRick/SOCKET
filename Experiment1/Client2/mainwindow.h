#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <time.h>
#include <winsock2.h>
#include <string>
#include <QMainWindow>


#define SERVER_ADDRESS "127.0.0.1" //服务器端IP地址
#define PORT           4000         //服务器的端口号
#define MSGSIZE        1024         //收发缓冲区的大小


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    WSADATA wsaData;//定义WSADATA结构体对象
    SOCKET ServerSocket;//s
    SOCKADDR_IN ServerAddr;
    //接受缓冲区
    char RecvBuffer[MAX_PATH],SendBuffer[MAX_PATH];
    //成功接收字节的个数
    int Ret;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
