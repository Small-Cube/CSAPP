 #include "csapp.h"
 void echo(int connfd);
 void *thread(void *vargp);
int main(int argc, char **argv)
{
    int listenfd, connfd, port, clientl
    int *connfdp;
//  static pool pool;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    pthread_t tid;
    fd_set read_set,ready_set;
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);
    // Signal(SIGCHLD,sigchld_handler);
    listenfd = Open_listenfd(port);
    while (1)
     {
 
         clientlen=sizeof(struct sockaddr_in);
         connfdp=Malloc(sizeof(int));
         *connfdp=Accept(listenfd,(SA*)&clientaddr,&clientlen);
         Pthread_create(&tid,NULL,thread,connfdp);
     }
 
 }
 
 void *thread(void *vargp)
 {
     int connfd=*((int *)vargp);
     Pthread_detach(pthread_self());
     Free(vargp);
     echo(connfd);
     Close(connfd);
     return NULL;
 }     
            
