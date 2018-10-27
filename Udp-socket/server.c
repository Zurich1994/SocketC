#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888 //定义端口号：（0-1024为保留端口号，最好不要用）
#define MAX_MSG_SIZE 1024 

void udps_respon(int sockfd) 
{ 
    struct sockaddr_in addr; 
    int addrlen,n; 
    char msg[MAX_MSG_SIZE]; 

    while(1) 
    {    /* 从网络上读,并写到网络上 */ 
        bzero(msg,sizeof(msg)); // 初始化,清零
        addrlen = sizeof(struct sockaddr); 
        n=recvfrom(sockfd,msg,MAX_MSG_SIZE,0,(struct sockaddr*)&addr,&addrlen); // 从客户端接收消息
        msg[n]=0; 
        /* 显示服务端已经收到了信息 */ 
        fprintf(stdout,"Server have received %s",msg); // 显示消息
    } 
} 

int main(void) 
{ 
    int sockfd; //socket描述符
    struct sockaddr_in addr; //定义服务器起地址

    /* 服务器端开始建立socket描述符 */ 
    sockfd=socket(AF_INET,SOCK_DGRAM,0); 
    if(sockfd<0) 
    { 
        fprintf(stderr,"Socket Error:%s\n",strerror(errno)); 
        exit(1); 
    } 

    /* 服务器端填充 sockaddr结构 */ 
    bzero(&addr,sizeof(struct sockaddr_in)); // 初始化,置0
    addr.sin_family=AF_INET; // Internet
    addr.sin_addr.s_addr=htonl(INADDR_ANY); // (将本机器上的long数据转化为网络上的long数据)和任何主机通信 //INADDR_ANY 表示可以接收任意IP地址的数据，即绑定到所有的IP
    //addr.sin_addr.s_addr=inet_addr("192.168.1.1"); //用于绑定到一个固定IP,inet_addr用于把数字加格式的ip转化为整形ip
    addr.sin_port=htons(SERVER_PORT); // (将本机器上的short数据转化为网络上的short数据)端口号

    /* 捆绑sockfd描述符 */ 
    if(bind(sockfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in))<0) 
    { 
        fprintf(stderr,"Bind Error:%s\n",strerror(errno)); 
        exit(1); 
    } 

    udps_respon(sockfd); // 进行读写操作
    close(sockfd); 
}
