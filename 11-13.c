#include"unp.h"
#include<time.h>

int main1113(int argc,char **argv)
{
    int listenfd,connfd;
    socklen_t len;
    char buf[MAXLINE];
    time_t ticks;
    struct sockaddr_storage cliaddr;
    if(argc!=2)
        err_quit("input argments error");

    listenfd=tcp_listen(NULL,argv[1],NULL);
    for(;;)
    {
        len=sizeof(cliaddr);
        connfd=Accept(listenfd,(SA *)&cliaddr,&len);
        printf("connection from %s\n",sock_ntop((SA *)&cliaddr,len));
        ticks=time(NULL);
        snprintf(buf,MAXLINE,"%.24s\r\n",ctime(&ticks));
        Write(connfd,buf,strlen(buf));
        Close(connfd);
    }
}
