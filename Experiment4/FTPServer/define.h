#ifndef DEFINE_H
#define DEFINE_H


//服务器侦听控制连接请求的端口
#define CMD_PORT 1333
//客户机侦听数据连接请求的端口
#define DATA_PORT 1334
//命令报文参数缓存的大小
#define CMD_PARAM_SIZE 256
//回复报文消息缓存的大小
#define RSPNS_TEXT_SIZE 256
#define BACKLOG 10


char filename[DATA_BUFSIZE];
char text[RSPNS_TEXT_SIZE];

//创建线程时传递的数据结构,内含控制连接套接字和客户端地址信息:
struct threadData{
    SOCKET soc;
    sockaddr_in clientaddr;
};

#endif // DEFINE_H
