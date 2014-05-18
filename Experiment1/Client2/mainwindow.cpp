#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    // 并关闭监听Socket,然后退出应用程序
    closesocket(ServerSocket);
    WSACleanup();
    // 释放Windows Socket DLL的相关资源
    delete ui;
}

//void MainWindow::on_pushButton_clicked()
//{

//    char p[MAX_PATH] = "Time";
//    strcpy(SendBuffer,p);
//    memset(SendBuffer, 0x00, sizeof(SendBuffer));
//    Ret = send(ServerSocket,SendBuffer,strlen(SendBuffer),0);
//    if ( Ret == 0 || Ret == SOCKET_ERROR )
//    {
//        cout<<"Server exist!"<<Ret<<endl;
//        return;
//    }
//   //send(ServerSocket, szMessage, strlen(szMessage), 0); //s指明用哪个连接发送； szMessage指明待发送数据的保存地址 ；strlen(szMessage)指明数据长度

   // 向服务器发出连接请求
   //WinSock2::




   // 新的连接建立后,就可以互相通信了,在这个简单的例子中,我们直接关闭连接,

//}


void MainWindow::on_pushButton_clicked()
{
    Ret = 0;
   //Init Windows Socket 2.2

   if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0){
       cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;

   }

   // Create Socket

   ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if ( ServerSocket == INVALID_SOCKET )
   {
       cout<<"Create Socket Failed::"<<GetLastError()<<endl;

   }
   cout<<"Client starts"<<endl;

   //AF_INET指明使用TCP/IP协议族；
   //SOCK_STREAM, IPPROTO_TCP具体指明使用TCP协议
   // 指明远程服务器的地址信息(端口号、IP地址等)

   //memset(&ServerAddr, 0, sizeof(SOCKADDR_IN)); //先将保存地址的server置为全0

   // 填写客户端地址信息
   // 端口为PORT
   // 服务器IP地址为本机ip,注意使用inet_addr将IP地址转换为网络格式

   ServerAddr.sin_family = AF_INET;
   ServerAddr.sin_port = htons(PORT);
   ServerAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
   WINSOCK_API_LINKAGE::connect(ServerSocket, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr));
   memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
   Ret = recv(ServerSocket, RecvBuffer, 16, 0);
   if ( Ret == 0 || Ret == SOCKET_ERROR )
   {
       cout<<"Server exist!"<<Ret<<endl;
   }
   cout<<"Client message:"<<RecvBuffer<<endl;
   ui->label_2->setText(RecvBuffer);


}
