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
#define MAX_BUF_SIZE 1024 

void udpc_requ(int sockfd,const struct sockaddr_in *addr,int len) 
{ 
    char buffer[MAX_BUF_SIZE]; 
    int n; 
    while(1) 
    {     /* 从键盘读入,写到服务端 */ 
        printf("Please input char:\n");
        fgets(buffer,MAX_BUF_SIZE,stdin); 
        sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)addr,len); 
        bzero(buffer,MAX_BUF_SIZE); 
    } 
} 

int main(int argc,char **argv) 
{ 
    int sockfd; //socket描述符
    struct sockaddr_in addr; //定义服务器起地址

    if(argc!=2) 
    { 
        fprintf(stderr,"Usage:%s server_ip\n",argv[0]); 
        exit(1); 
    }

    /* 建立 sockfd描述符 */ 
    sockfd=socket(AF_INET,SOCK_DGRAM,0); 
    if(sockfd<0) 
    { 
        fprintf(stderr,"Socket Error:%s\n",strerror(errno)); 
        exit(1); 
    } 

    /* 填充服务端的资料 */ 
    bzero(&addr,sizeof(struct sockaddr_in)); // 初始化,置0
    addr.sin_family=AF_INET; // Internet
    addr.sin_port=htons(SERVER_PORT);// (将本机器上的short数据转化为网络上的short数据)端口号
    if(inet_aton(argv[1],&addr.sin_addr)<0) /*inet_aton函数用于把字符串型的IP地址转化成网络2进制数字*/ 
    { 
        fprintf(stderr,"Ip error:%s\n",strerror(errno)); 
        exit(1); 
    } 

    udpc_requ(sockfd,&addr,sizeof(struct sockaddr_in)); // 进行读写操作
    close(sockfd); 
}
