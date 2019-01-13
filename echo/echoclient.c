//CSAPP中的一些辅助函数的定义
#include "csapp.h"  
  
int main(int argc, char **argv)   
{  
 
    int clientfd, port;  
    char *host, buf[MAXLINE];  
    rio_t rio;  
    // 客户端需要传进参数，IP地址和端口
    if (argc != 3)   
    {  
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);  
        exit(0);  
    }  
    host = argv[1];  
    //将字符串转为int，书上没有转换是错的
    port = atoi(argv[2]);  
    //打开
    clientfd = Open_clientfd(host, port);  
   //初始化RIO读写
    Rio_readinitb(&rio, clientfd);  
  
    while (Fgets(buf, MAXLINE, stdin) != NULL)   
    {  
        
        Rio_writen(clientfd, buf, strlen(buf));  
        Rio_readlineb(&rio, buf, MAXLINE);  
        Fputs(buf, stdout);  
    }  
    Close(clientfd); //line:netp:echoclient:close  
    exit(0);  
}  
