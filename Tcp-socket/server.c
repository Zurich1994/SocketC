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
    int sockfd,new_fd; 
    struct sockaddr_in server_addr; //描述服务器地址
    struct sockaddr_in client_addr; //描述客户端地址
    int sin_size; 
    char hello[]="Hello! Are You Fine?\n"; 
    

    /* 服务器端开始建立sockfd描述符 */ 
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:IPV4;SOCK_STREAM:TCP
    { 
        fprintf(stderr,"Socket error:%s\n\a",strerror(errno)); 
        exit(1); 
    } 

    /* 服务器端填充 sockaddr结构 */ 
    bzero(&server_addr,sizeof(struct sockaddr_in)); // 初始化,置0
    server_addr.sin_family=AF_INET; // Internet
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY); // (将本机器上的long数据转化为网络上的long数据)和任何主机通信 //INADDR_ANY 表示可以接收任意IP地址的数据，即绑定到所有的IP
    //server_addr.sin_addr.s_addr=inet_addr("192.168.1.1"); //用于绑定到一个固定IP,inet_addr用于把数字加格式的ip转化为整形ip
    server_addr.sin_port=htons(portnumber); // (将本机器上的short数据转化为网络上的short数据)端口号
    
    /* 捆绑sockfd描述符到IP地址 */ 
    if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1) 
    { 
        fprintf(stderr,"Bind error:%s\n\a",strerror(errno)); 
        exit(1); 
    } 

    /* 设置允许连接的最大客户端数 */ 
    if(listen(sockfd,5)==-1) 
    { 
        fprintf(stderr,"Listen error:%s\n\a",strerror(errno)); 
        exit(1); 
    } 

    while(1) 
    { 
        /* 服务器阻塞,直到客户程序建立连接 */ 
        sin_size=sizeof(struct sockaddr_in); 
        if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1) 
        { 
            fprintf(stderr,"Accept error:%s\n\a",strerror(errno)); 
            exit(1); 
        } 
        fprintf(stderr,"Server get connection from %s\n",inet_ntoa(client_addr.sin_addr)); // 将网络地址转换成.字符串，并打印到输出终端
        
        //向客户端程序写入hello数组里的字符
        if(write(new_fd,hello,strlen(hello))==-1) 
        { 
            fprintf(stderr,"Write Error:%s\n",strerror(errno)); 
            exit(1); 
        } 
        /* 这个通讯已经结束 */ 
        close(new_fd); 
        /* 循环下一个 */ 
    } 

    /* 结束通讯 */ 
    close(sockfd); 
    exit(0); 
}
