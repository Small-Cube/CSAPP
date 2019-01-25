#include "csapp.h"  
  
void echo(int connfd);
//信号处理程序
void sigchld_handler(int sig)
{
	while(waitpid(-1,0,WNOHANG)>0);
	return ;
}
int main(int argc, char **argv)   
{  
    int listenfd, connfd, port, clientlen;  
    struct sockaddr_in clientaddr;  
    struct hostent *hp;  
    char *haddrp;  
    if (argc != 2)   
    {  
        fprintf(stderr, "usage: %s <port>\n", argv[0]);  
        exit(0);  
    }  
    port = atoi(argv[1]);  
   Signal(SIGCHLD,sigchld_handler);
    listenfd = Open_listenfd(port);  
    while (1)   
    {  
        clientlen = sizeof(clientaddr);  
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);  
  
        /* determine the domain name and IP address of the client */  
        hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,   
                sizeof(clientaddr.sin_addr.s_addr), AF_INET);  
        haddrp = inet_ntoa(clientaddr.sin_addr);  
        printf("server connected to %s (%s)\n", hp->h_name, haddrp);  
        if(Fork()==0){
            //子进程关闭监听描述符
	        Close(listenfd);
            echo(connfd);  
            Close(connfd);
		    exit(0);
		}
		//父进程关闭已连接描述符
		Close(connfd);
    }  
   // exit(0);  
}  
