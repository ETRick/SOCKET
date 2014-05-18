#include <stdio.h>
#include <winsock2.h>
#include <iostream>


#define PORT_CTRL 1333   //控制端口
#define PORT_DATA 1334  //数据端口
#define DATA_BUFSIZE 4096
#define TEXT_SIZE 256

using namespace std;

//创建线程时传递的数据结构,内含控制连接套接字和客户端地址信息:
struct threadData{
    SOCKET soc;
    sockaddr_in clientaddr;
};

//线程函数,参数包括相应控制连接的套接字:
DWORD WINAPI myfun(LPVOID lpParam){
    SOCKET tcp_soc;
    int Ret;
    SOCKADDR_IN clientaddr,dataaddr;
    tcp_soc = ((struct threadData *)lpParam)->soc;
    clientaddr = ((struct threadData *)lpParam)->clientaddr;
    char filename[TEXT_SIZE];
    char text[TEXT_SIZE];
    cout<<"socket id is "<<tcp_soc<<endl;

    //返回给客户端的信息
    strcpy(text,"This is a Message from FTP Server");
    Ret = send(tcp_soc, (char *)text, sizeof(text), 0);


    //循环获取客户端命令报文并进行处理
    while(true){
        //首先建立数据连接:
        WIN32_FIND_DATA fd;
        SOCKET datasock;
        datasock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(datasock == INVALID_SOCKET)
        {
            cout<<"socket() Failed :"<<GetLastError()<<endl;
            return -1;
        }
        // ftp_addr.sin_family = AF_INET;
        // ftp_addr.sin_port = htons(PORT_DATA);
        // ftp_addr.sin_addr.s_addr = INADDR_ANY;

        memcpy(&dataaddr, &clientaddr, sizeof(SOCKADDR_IN));
        dataaddr.sin_port = htons(PORT_DATA);//端口不同

       
        Ret = recv(tcp_soc, filename, TEXT_SIZE, 0);
        connect(datasock, (SOCKADDR*)&dataaddr,sizeof(dataaddr));
        //printf("Serve client %s:%d\n",inet_ntoa(dataaddr.sin_addr),ntohs(dataaddr.sin_port));

        FILE *file = fopen(filename,"rb");//没有自己的属性了
        if(file)
        {
            //int SendFile(SOCKET datasock, FILE* file){
            char buf[DATA_BUFSIZE];
            cout<<"sending file data..."<<"filename "<<filename<<endl;
            while(TRUE){                //从文件中循环读取数据并发送客户端
                int r = fread(buf, 1,DATA_BUFSIZE,file);
                cout<<buf<<endl;
                if(send(datasock, buf, DATA_BUFSIZE, 0)== SOCKET_ERROR){
                    
                    cout<<"lost the connection to client !  "<<GetLastError()<<endl;
                    closesocket(datasock);
                    return 0;
                }
                   
                if(r<DATA_BUFSIZE)   //文件传输结束
                {
                    break;
                }
            }//while
            fclose(file);
            printf("done!\n");
            return 1;
        }//if file
        else  //打开文件失败
        {
            cout<<"no such file"<<endl;
            strcpy(text, "can't open file!");
            send(tcp_soc, (char *)text, sizeof(text), 0);
        }
        closesocket(datasock);
  }
}





int main(int argc, char* argv[]){
    WSADATA  Ws;
    SOCKADDR_IN ftp_addr;
    SOCKET ftp_listen;
    int Ret;
    int threadData_len = sizeof(struct threadData);

    //初始化WinSock 资源
    if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
    {
        cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
        return -1;
    }
    //创建SOCKET
    ftp_listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ftp_listen == INVALID_SOCKET)
    {
        cout<<"socket() Failed :"<<GetLastError()<<endl;
        return -1;
    }

    ftp_addr.sin_family = AF_INET;
    ftp_addr.sin_port = htons(PORT_CTRL);
    ftp_addr.sin_addr.s_addr = INADDR_ANY;

    //绑定套接字
    Ret = bind(ftp_listen, (SOCKADDR*)&ftp_addr,sizeof(ftp_addr));
    if ( Ret != 0 )
    {
         cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
         return -1;
    }
    //监听套接字
    Ret = listen(ftp_listen, 3);
    if ( Ret != 0 )
    {
        cout<<"listen Socket Failed::"<<GetLastError()<<endl;
        return -1;
    }
    //服务器启动成功输出
    cout<<"Server has been started"<<endl;


    struct threadData *td;//threaddata 指针

    while(true){
        td = new threadData;
        if(td == NULL){
            cout<<"Malloc Space Failed!\n";
            continue;
        }

        //等待接受客户端控制连接请求
        td->soc = accept(ftp_listen,(SOCKADDR*)&td->clientaddr,&threadData_len);

        //用线程来处理相应客户端请求
        DWORD dwThreadId, dwThrdParam = 1;
        HANDLE hThread;
        //调用WinAPI
        hThread = CreateThread( NULL, 0,myfun,td,0,&dwThreadId);
        if(hThread == NULL)
        {
            cout<<"CreateThread Failed"<<endl;
        }
    }
    return 0;
}
