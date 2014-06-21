#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#define IP_ADDRESS "127.0.0.1"
#define DATA_BUFSIZE 4096
#define PORT_CTRL 1333
#define PORT_DATA 1334

using namespace std;
int main()
{
	WSADATA  Ws;
	SOCKET ServerSocket,ListenSocket,DataSocket;
	struct sockaddr_in LocalAddr,ServerAddr;
	int Ret = 0;
	int filelen; //file length
	char fileName[50];
	char RecvBuffer[DATA_BUFSIZE];
	char statusBuf[100];

	//Winsock init
	if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
    {
        cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
        return -1;
    }

    cout<<"INIT SUCCESS��"<<endl;
	//Create socket
	ServerSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if ( ServerSocket == INVALID_SOCKET )
    {
        cout<<"Create Socket Failed::"<<GetLastError()<<endl;
        return -1;
    }

	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_port = htons(PORT_CTRL);
	LocalAddr.sin_addr.S_un.S_addr=inet_addr(IP_ADDRESS);

	
	//Connect
	Ret = connect(ServerSocket,(struct sockaddr*)&LocalAddr,sizeof(struct sockaddr));
	if(Ret == SOCKET_ERROR)
	{
		cout<<"Connect Failed"<<GetLastError()<<endl;
		return -1;
	}

	//Receive message
	recv(ServerSocket,RecvBuffer,DATA_BUFSIZE,0);
	cout<<RecvBuffer<<endl;


	cout<<"Please enter fileName :(enter 'q' to quit)"<<endl;
	cin>>fileName;cout<<endl;
	cout<<"your fileName is  :"<<fileName<<endl;
	if(strcmp(fileName,"q") == 0)
	{
		cout<<"Quit ...."<<endl;
	}
	else
    {
    	filelen = strlen(fileName) + 1;
		send(ServerSocket,fileName,filelen,0);
	
		
		//Create listen socket
		ListenSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if ( ListenSocket == INVALID_SOCKET )
		{
		    cout<<"Create Socket Failed::"<<GetLastError()<<endl;
		    return -1;
		}

		SOCKADDR_IN addr_in;
		addr_in.sin_family=AF_INET;
		addr_in.sin_port=htons(PORT_DATA);
		addr_in.sin_addr.S_un.S_addr=inet_addr(IP_ADDRESS);

		//Bind
		Ret = bind(ListenSocket, (sockaddr*)&addr_in, sizeof(sockaddr));
		if ( Ret != 0 )
		{
		   	cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
		    return -1;
		}

		//Listen
		Ret = listen(ListenSocket,3);
		if ( Ret != 0 )
		{
		    cout<<"listen Socket Failed::"<<GetLastError()<<endl;
		    return -1;
		}


		int length=sizeof(SOCKADDR);
		//Start receiving
		cout<<"Receiving1 ......."<<endl;

		DataSocket=accept(ListenSocket,(SOCKADDR*)&addr_in,&length);
		cout<<GetLastError()<<endl;

		cout<<"Receiving0 ......."<<endl;

		int byteSize=0;
		cout<<"before recv text"<<endl;
		byteSize=recv(DataSocket,RecvBuffer,DATA_BUFSIZE,0);
		cout<<"RecvBuffer"<<RecvBuffer<<endl;
		//Write data received from server into file
		FILE* outfp;
		if((outfp=fopen(fileName,"w"))==NULL)
		{
			cout<<"Can't write!"<<endl;
			return -1;
		}
		if(byteSize>0)
		{
			cout<<"File Receive SUCCESS!"<<endl;
			cout<<"File Size: "<<byteSize<<endl;
			cout<<"File Content : "<<RecvBuffer<<endl;
			fputs(RecvBuffer,outfp);//byteSize
		}
		closesocket(ListenSocket);
		closesocket(DataSocket);
	}

	WSACleanup();
	closesocket(ServerSocket);
	return 0;
}
