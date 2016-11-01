#include "unp.h"
#include <time.h>


int main411()
{
    int listenfd,connfd;
    socklen_t len;

    struct sockaddr_in servaddr,cliaddr;
    char buff[MAXLINE];
    time_t ticks;

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
        connfd=Accept(listenfd,&cliaddr,&len);
        printf("connect from %s ,port %d\n",
               inet_ntop(AF_INET,&cliaddr.sin_addr,buff,sizeof(buff)),
               ntohs(cliaddr.sin_port));

        ticks=time(NULL);
        snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
        Write(connfd,buff,strlen(buff));
        Close(connfd);
    }
}
