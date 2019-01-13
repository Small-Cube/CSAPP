#include "csapp.h"  
//服务器为客户端服务函数
void echo(int connfd);  
  
int main(int argc, char **argv)   
{  
    int listenfd, connfd, port, clientlen;  
    struct sockaddr_in clientaddr;  
    struct hostent *hp;  
    char *haddrp;  
    //需要传入一个端口号
    if (argc != 2)   
    {  
        fprintf(stderr, "usage: %s <port>\n", argv[0]);  
        exit(0);  
    }  
    //字符串转换为int
    port = atoi(argv[1]);  
     //打开一个监听描述符
    listenfd = Open_listenfd(port);  
    while (1)   
    {  
        clientlen = sizeof(clientaddr); 
       // 服务器通过调用accept函数来等待来自客户端的连接请求
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);  
  
        /* determine the domain name and IP address of the client */  
        hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,   
                sizeof(clientaddr.sin_addr.s_addr), AF_INET);  
        haddrp = inet_ntoa(clientaddr.sin_addr);  
        printf("server connected to %s (%s)\n", hp->h_name, haddrp);  
  
        echo(connfd);  
        Close(connfd);  
    }  
    exit(0);  
}  
//服务函数
void echo(int connfd)   
{  
    size_t n;   
    char buf[MAXLINE];   
    rio_t rio;  
  
    Rio_readinitb(&rio, connfd);  
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)   
    { //line:netp:echo:eof  
        printf("server received %d bytes\n", n);  
        Rio_writen(connfd, buf, n);  
    }  
}  
