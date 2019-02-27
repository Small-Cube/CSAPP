#include "csapp.h"
typedef struct{
    int maxfd;
    fd_set read_set;
    fd_set ready_set;
    int nready;
    int maxi;
    int clientfd[FD_SETSIZE];
    rio_t clientrio[FD_SETSIZE];
}pool;
int byte_cnt=0;

void echo(int connfd);
void init_pool(int listenfd,pool *p)
{
    int i;
    p->maxi=-1;
    for(i=0;i<FD_SETSIZE;i++)
      p->clientfd[i]=-1;
    p->maxfd=listenfd;
    FD_ZERO(&p->read_set);
    FD_SET(listenfd,&p->read_set);
}
void add_client(int connfd,pool *p)
{
    int i;
    p->nready--;
    for(int i=0;i<FD_SETSIZE;i++)
      if(p->clientfd[i]<0){
          p->clientfd[i]=connfd;
          Rio_readinitb(&p->clientrio[i],connfd);
          FD_SET(connfd,&p->read_set);
          if(connfd>p->maxfd)
            p->maxfd=connfd;
          if(i>p->maxi)
            p->maxi=i;
          break;


      }
     if(i==FD_SETSIZE)
       app_error("add_client error :to many clients");
 }
 
 void check_clients(pool *p)
 {
     int i,connfd,n;
     char buf[MAXLINE];
     rio_t rio;
     for(i=0;(i<= p->maxi)&&(p->nready > 0);i++){
       connfd=p->clientfd[i];
       rio = p->clientrio[i];
 
       if((connfd > 0)&&(FD_ISSET(connfd,&p->ready_set))){
           p->nready--;
           if((n=Rio_readlineb(&rio,buf,MAXLINE))!=0){
               byte_cnt+=n;
               printf("Server receive %d (%d total) bytes on fd %d\n",n,byte_cnt,connfd);
               Rio_writen(connfd,buf,n);
           }
           else
           {
               Close(connfd);
               FD_CLR(connfd,&p->read_set);
               p->clientfd[i]=-1;
           }
       }
     }
 
 
 }
 int main(int argc, char **argv)
 {
     int listenfd, connfd, port, clientlen;
     static pool pool;
     struct sockaddr_in clientaddr;
     struct hostent *hp;
     char *haddrp;
     fd_set read_set,ready_set;
     if (argc != 2)
     {
         fprintf(stderr, "usage: %s <port>\n", argv[0]);
         exit(0);
     }
     port = atoi(argv[1]);
     // Signal(SIGCHLD,sigchld_handler);
     listenfd = Open_listenfd(port);
 
     init_pool(listenfd,&pool);
 //  FD_ZERO(&read_set);
 //  FD_SET(STDIN_FILENO,&read_set);
 //  FD_SET(listenfd,&read_set);
     while (1)
     {
 
         pool.ready_set=pool.read_set;
         pool.nready=Select(pool.maxfd+1,&pool.ready_set,NULL,NULL,NULL);
         //if(FD_ISSET(STDIN_FILENO,&read_set)){
         //  command();   //从标准输入读命令行
         //}
         if(FD_ISSET(listenfd,&pool.ready_set)){
              clientlen = sizeof(clientaddr);
              connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
              add_client(connfd,&pool);
         /* determine the domain name and IP address of the client */
              /* hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr
               sizeof(clientaddr.sin_addr.s_addr), AF_INET);  
               haddrp = inet_ntoa(clientaddr.sin_addr);  
               printf("server connected to %s (%s)\n", hp->h_name, haddrp);  
               echo(connfd);  
              Close(connfd);*/



        }
        check_clients(&pool);

    }