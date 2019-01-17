#include"csapp.h"
#define N 2
int main()
{
    int status,i;
    pid_t pid[N],retpid;
   //创建N个子进程
    for(i=0;i<N;i++)
      if((pid[i]=Fork())==0)   //子进程
      //退出子进程
        exit(100+i);
    //waitpid第一个参数为-1，等待集合是父进程的所有子进程
    i=0;
    while((retpid=waitpid(pid[i],&status,0))>0){
        if(WIFEXITED(status))
            printf("child %d terminated normally with exit status=%d\n",retpid,WEXITSTATUS(status));
        else
          printf("child %d terminated abnormally \n",retpid);
    }

    if(errno!=ECHILD)
      unix_error("waitpid error");
    exit(0);
}
