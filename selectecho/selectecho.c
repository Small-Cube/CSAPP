 #include "csapp.h"
 
 void echo(int connfd);
 void command(void);
 
 int main(int argc, char **argv)
 {
     int listenfd, connfd, port, clientlen;
     struct sockaddr_in clientaddr;
     struct hostent *hp;
     char *haddrp;
     fd_set read_set,ready_set;
     if (argc != 2)
     {
         fprintf(stderr, "usage: %s <port>\n", argv[0])
         exit(0);
     }
     port = atoi(argv[1]);
     // Signal(SIGCHLD,sigchld_handler);
     listenfd = Open_listenfd(port);
     FD_ZERO(&read_set);
     FD_SET(STDIN_FILENO,&read_set);
     FD_SET(listenfd,&read_set);
     while (1)
     {
         ready_set=read_set;
         Select(listenfd+1,&read_set,NULL,NULL,NULL);
       
       if(FD_ISSET(STDIN_FILENO,&read_set)){
           command();   //从标准输入读命令行
       }
       if(FD_ISSET(listenfd,&ready_set)){
            clientlen = sizeof(clientaddr);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
 
       /* determine the domain name and IP address of the client */
             hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
             sizeof(clientaddr.sin_addr.s_addr), AF_INET);
             haddrp = inet_ntoa(clientaddr.sin_addr);
             printf("server connected to %s (%s)\n", hp->h_name, haddrp);
             echo(connfd);
             Close(connfd);
 
 
       }
 
    }
 
}
 
void command(void)
{
    char buf[MAXLINE];
    if(!Fgets(buf,MAXLINE,stdin))
      exit(0);
    printf("%s",buf);
}
