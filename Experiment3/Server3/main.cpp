#include <iostream>
#include <stdio.h>
#include <Winsock2.h>
#include <WS2tcpip.h>
#include "headers.h"

using namespace std;
//包括IP首部在内的数据长度不能超过65535
#define MSGSIZE 65535
#define  PORT 4000
#define SIO_RCVALL 0x80000000|(0x18000000)|(1)
int main()
{
	WSADATA  Ws;
    int Ret = 0;
	unsigned timeout;
	struct hostent *host_ent = NULL;
	SOCKET sock;
    struct sockaddr_in from_addr;
	char RecvBuffer[MSGSIZE];
    ip_hdr ip_h;
    tcp_hdr tcp_h;

	 //初始化WinSock 资源
	if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
    {
        cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
        return -1;
    }

    char  LocalName[16];
    // 获取本机名
    Ret = gethostname((char*)LocalName, sizeof(LocalName)-1);
    if(Ret == SOCKET_ERROR)
    {
        cout<<"gethostname() Error! "<<GetLastError()<<endl;
        return -1;
    }

    // 获取本地 IP 地址
    if((host_ent = gethostbyname((char*)LocalName)) == NULL)
    {
        cout<<"gethostbyname error! "<<GetLastError()<<endl;
        return -1;
    }


    sock = socket(AF_INET,SOCK_RAW,IPPROTO_IP);
    if(sock == INVALID_SOCKET)
    {
    	cout<<"socket() Failed :"<<GetLastError()<<endl;
    	return -1;
    }

    BOOL blnFlag=TRUE;
     Ret = setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *)&blnFlag, sizeof(blnFlag));
     if(Ret < 0 ){
        cout<<"setsockopt() error1 "<<GetLastError()<<endl;
     }
     timeout = 1000;
     //设置套接字选项
     Ret = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
     if(Ret < 0 ){
         cout<<"setsockopt() error2 :"<<GetLastError()<<endl;
     }

    from_addr.sin_addr  = *(in_addr *)host_ent->h_addr_list[0]; // IP
    from_addr.sin_family = AF_INET;
    from_addr.sin_port  = htons(PORT);

    
    //绑定SOCKET
    Ret = bind(sock, (struct sockaddr*)&from_addr, sizeof(from_addr));
    if ( Ret != 0 )
    {
         cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
         return -1;
    }

    DWORD dwValue = 1;
    //控制口模式
    if( ioctlsocket(sock, SIO_RCVALL, &dwValue) != 0)
    {
        printf("ioctlsocket error! %d", WSAGetLastError());
        return 1;
    }
    //输出提示服务已启动
    cout<<"Service starts......."<<endl<<endl<<endl<<endl<<endl<<endl;


    while(true)
    {
        memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
        //recvfrom之前必须先调用bind，sendto之类的
		Ret = recv(sock, RecvBuffer, MSGSIZE, 0);

		if ( Ret == 0 || Ret == SOCKET_ERROR )
		{
		    cout<<"recv Failed: "<<GetLastError()<<endl;
		    break;
		}


		if(Ret > 0)
        {
            //ip头
            ip_h = *(ip_hdr *)RecvBuffer;
            //IP头长度
            int iIphLen = sizeof(unsigned long) * (ip_h.ip_vers_len & 0xf);

            if(ip_h.ip_proto == IPPROTO_TCP)
            {
                tcp_h = *(struct tcp_hdr*)(RecvBuffer + iIphLen);
                char *ptr = RecvBuffer + iIphLen + (4 * ((tcp_h.tcp_lenres & 0xf0)>>4|0));

                cout<<"Applications+++++++++++"<<endl<<endl;
                cout<<"Data :"<<ptr<<endl<<endl;
            
                cout<<"Transportation+++++++++++"<<endl<<endl;
                cout<<"SRC_IP : "<<inet_ntoa(*(in_addr*)&ip_h.ip_sour)<<endl;
                cout<<"DES_IP : "<<inet_ntoa(*(in_addr*)&ip_h.ip_dest)<<endl;


                cout<<"Network+++++++++++"<<endl<<endl;

                
                cout<<"SRC_TCP_PORT : "<<tcp_h.tcp_sport<<endl;
                cout<<"DES_TCP_PORT : "<<tcp_h.tcp_dport<<endl<<endl<<endl<<endl<<endl<<endl<<endl;

            }
        }


    }












//tcp=(struct tcp *)(buffer+(4*ip->ip_length))



	return 0;

}

