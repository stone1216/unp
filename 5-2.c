#include "unp.h"
#include <time.h>


static str_echo(int fd)
{
    ssize_t n;
    char buf[MAXLINE];
    again:
    while((n=read(fd,buf,MAXLINE))>0)
        Writen(fd,buf,n);
    if(n<0&&errno==EINTR)
        goto again;
    else if(n<0)
        err_sys("str_echo : read error");
}

static void sig_child(int signo)
{
    pid_t pid;
    int stat;

    while((pid=waitpid(-1,&stat,WNOHANG))>0)
        printf("child %d terminate\n",pid);

    return ;
}

int main52()
{
    int listenfd,connfd;
    pid_t childpid;
    socklen_t len;

    struct sockaddr_in servaddr,cliaddr;

    signal(SIGCHLD,sig_child);

    listenfd=Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(5555);
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);

    Bind(listenfd,(SA *) &servaddr,sizeof(servaddr));
    Listen(listenfd,LISTENQ);

    for(;;)
    {
        len=sizeof(cliaddr);
        if((connfd=accept(listenfd,&cliaddr,&len))<0)
        {
            if(errno==EINTR)
                continue;
            else
                err_sys("accept error");
        }
        if((childpid=fork())==0)
        {
            Close(listenfd); 
            str_echo(connfd);
            Close(connfd);
            exit(0);
        }
        Close(connfd);

    }
}
