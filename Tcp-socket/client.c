#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> //inet_ntoa()函数的头文件


#define portnumber 3333 //定义端口号：（0-1024为保留端口号，最好不要用）

int main(int argc, char *argv[]) 
{ 
    int sockfd; 
    char buffer[1024]; 
    struct sockaddr_in server_addr; //描述服务器的地址
    struct hostent *host; 
    int nbytes; 

    /* 使用hostname查询host 名字 */
    if(argc!=2) 
    { 
        fprintf(stderr,"Usage:%s hostname \a\n",argv[0]); 
        exit(1); 
    } 

    if((host=gethostbyname(argv[1]))==NULL) 
    { 
        fprintf(stderr,"Gethostname error\n"); 
        exit(1); 
    } 

    /* 客户程序开始建立 sockfd描述符 */ 
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:Internet;SOCK_STREAM:TCP
    { 
        fprintf(stderr,"Socket Error:%s\a\n",strerror(errno)); 
        exit(1); 
    } 

    /* 客户程序填充服务端的资料 */ 
    bzero(&server_addr,sizeof(server_addr)); // 初始化,置0
    server_addr.sin_family=AF_INET; // IPV4
    server_addr.sin_port=htons(portnumber); // (将本机器上的short数据转化为网络上的short数据)端口号
    server_addr.sin_addr=*((struct in_addr *)host->h_addr); // IP地址
    
    /* 客户程序发起连接请求 */ 
    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1) 
    { 
        fprintf(stderr,"Connect Error:%s\a\n",strerror(errno)); 
        exit(1); 
    } 

    /* 连接成功了 */ 
    if((nbytes=read(sockfd,buffer,1024))==-1) 
    { 
        fprintf(stderr,"Read Error:%s\n",strerror(errno)); 
        exit(1); 
    } 
    buffer[nbytes]='\0'; 
    printf("I have received:%s\n",buffer); 

    /* 结束通讯 */ 
    close(sockfd); 
    exit(0); 
}
