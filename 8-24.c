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

int main824()
{
    int listenfd,connfd,udpfd,maxfdp1,nready,n;
    pid_t childpid;
    socklen_t len;
    fd_set rset;

    char buf[MAXLINE];

    struct sockaddr_in servaddr,cliaddr;

    signal(SIGCHLD,sig_child);

    listenfd=Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(5555);
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);

    Bind(listenfd,(SA *) &servaddr,sizeof(servaddr));
    Listen(listenfd,LISTENQ);

    //create udp socket
    udpfd=Socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(5555);
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);

    Bind(udpfd,(SA *)&servaddr,sizeof(servaddr));


    FD_ZERO(&rset);
    maxfdp1=max(listenfd,udpfd)+1;
    for(;;)
    {
        FD_SET(listenfd,&rset);
        FD_SET(udpfd,&rset);
        nready=Select(maxfdp1,&rset,NULL,NULL,NULL);

        if(FD_ISSET(listenfd,&rset))
        {
            len=sizeof(cliaddr);
            connfd=Accept(listenfd,&cliaddr,&len);
            if((childpid=fork())==0)
            {
                Close(listenfd);
                str_echo(connfd);
                Close(connfd);
                exit(0);
            }
            Close(connfd);
        }

        if(FD_ISSET(udpfd,&rset))
        {
            len=sizeof(cliaddr);
            n=Recvfrom(udpfd,buf,MAXLINE,0,(SA *)&cliaddr,&len);
            //printf("receive %d\n",n);
            Sendto(udpfd,buf,n,0,(SA *)&cliaddr,len);
        }

    }
}
