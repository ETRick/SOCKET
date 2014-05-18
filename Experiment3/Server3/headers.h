#ifndef HEADERS_H
#define HEADERS_H
/*IP首部*/
struct ip_hdr
{
    unsigned char ip_vers_len;      	//版本和首部长度
    unsigned char ip_tos;               //服务类型
    unsigned short ip_total_len;		//数据包的总长度
    unsigned short ip_id;				//标识符
    unsigned short ip_frag;				//标识符和片偏移
    unsigned char ip_ttl;				//生存时间
    unsigned char ip_proto;				//协议
    unsigned short ip_checksum;			//校验和
    unsigned int ip_sour;				//源IP地址
    unsigned int ip_dest;				//目的IP地址
};

/*ICMP 首部*/
struct icmp_hdr
{
    unsigned char icmp_type;			//类型
    unsigned char icmp_code;			//代码
    unsigned short icmp_checksum;		//校验和
    unsigned short icmp_id;				//标识符
    unsigned short icmp_seq;			//序列号
    unsigned long icmp_timestamp;		//用于记录时间，非标准
};

/*TCP 首部*/
struct tcp_hdr
{
    unsigned short   tcp_sport;  		//16位源端口
    unsigned short   tcp_dport;    		//16位目的端口
    unsigned int     tcp_seq;      		//32位序列号
    unsigned int     tcp_ack;     		//32位确认号
    unsigned char    tcp_lenres; 		//4位首部长度/6位保留字
    unsigned char    tcp_flag;     		//6位标志位
    unsigned short   tcp_win;      		//16位窗口大小
    unsigned short   tcp_sum;       	//16位校验和
    unsigned short   tcp_urp;       	//16位紧急数据偏移量
};



#endif // HEADERS_H
