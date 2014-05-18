#include <iostream>
#include <windows.h>
#include <sys/types.h>
#include <winsock2.h>
#include <stdlib.h>
#include <ctime>
#include <time.h>
using namespace std;

#define  PORT 4000
#define  IP_ADDRESS "127.0.0.1"
#define MSGSIZE        1024         //收发缓冲区的大小

DWORD WINAPI myfun(LPVOID lpParameter);

int main()
{
      HANDLE h;
      WSADATA  Ws;
      SOCKET ServerSocket, CientSocket;
      struct sockaddr_in LocalAddr, ClientAddr;
      int Ret = 0;
      int AddrLen = 0;
      //初始化WinSock 资源
      if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
      {
          cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
          return -1;
      }

      //创建套接字
      ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if ( ServerSocket == INVALID_SOCKET )
      {
          cout<<"Create Socket Failed::"<<GetLastError()<<endl;
          return -1;
      }
      //启动输出
      cout<<"Server starts"<<endl;
      //指定地址和端口号
      LocalAddr.sin_family = AF_INET;
      LocalAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
      LocalAddr.sin_port = htons(PORT);
      memset(LocalAddr.sin_zero, 0x00, 8);

      //绑定套接字
      Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
      if ( Ret != 0 )
      {
          cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
          return -1;
      }
      //监听套接字
      Ret = listen(ServerSocket, 10);
      if ( Ret != 0 )
      {
          cout<<"listen Socket Failed::"<<GetLastError()<<endl;
          return -1;
      }
      //服务器启动成功输出
      cout<<"Server has been started"<<endl;

      while ( true )
      {
          AddrLen = sizeof(ClientAddr);
          //接收套接字
          CientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);
          if ( CientSocket == INVALID_SOCKET )
          {
              cout<<"Accept Failed::"<<GetLastError()<<endl;
              break;
          }
          // inet_ntoa：将一个IP转换成一个互联网标准点分格式的字符串
          // _strtime ：获取当前系统时间
          cout<<"Client connection::"<<inet_ntoa(ClientAddr.sin_addr)<<":"<<ClientAddr.sin_port<<endl;



          h = CreateThread(NULL, 0, myfun, (LPVOID)CientSocket, 0, NULL);
          if(h == NULL)
          {
              cout<<"Create Thread Failed! "<<endl;
              break;
          }
          CloseHandle(h);

          //send(CientSocket, tmpbuf, strlen(tmpbuf), 0);
          //CientSocket指明用哪个连接发送； tmpbuf指明待发送数据的保存地址 ；strlen(tmpbuf)指明数据长度


      }
      //关闭连接释放资源
      closesocket(ServerSocket);
      closesocket(CientSocket);
      WSACleanup();

      return 0;
 }


DWORD WINAPI myfun(LPVOID lpParameter)
{
  SOCKET CientSocket = (SOCKET)lpParameter;
  int Ret = 0;
  char RecvBuffer[MAX_PATH],tmpbuf[MSGSIZE];;

//  while ( true )
//  {
//    memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
//    Ret = recv(CientSocket, RecvBuffer, MAX_PATH, 0);
//    if ( Ret == 0 || Ret == SOCKET_ERROR )
//    {
//      cout<<"客户端退出!"<<endl;
//      break;
//    }
//    //cout<<"接收到客户信息为:"<<RecvBuffer<<endl;

    _strtime(tmpbuf);
    send(CientSocket, tmpbuf, strlen(tmpbuf), 0);
  // }
  return 0;
}
