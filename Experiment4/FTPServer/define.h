#ifndef DEFINE_H
#define DEFINE_H


//����������������������Ķ˿�
#define CMD_PORT 1333
//�ͻ�������������������Ķ˿�
#define DATA_PORT 1334
//����Ĳ�������Ĵ�С
#define CMD_PARAM_SIZE 256
//�ظ�������Ϣ����Ĵ�С
#define RSPNS_TEXT_SIZE 256
#define BACKLOG 10


char filename[DATA_BUFSIZE];
char text[RSPNS_TEXT_SIZE];

//�����߳�ʱ���ݵ����ݽṹ,�ں����������׽��ֺͿͻ��˵�ַ��Ϣ:
struct threadData{
    SOCKET soc;
    sockaddr_in clientaddr;
};

#endif // DEFINE_H
