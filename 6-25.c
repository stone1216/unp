#include "unp.h"
#include<limits.h>

#define OPEN_MAX 1024


int main625()
{
    int listenfd,connfd,i,maxi,sockfd;
    socklen_t len;

    char buf[MAXLINE];
    int nready,n;

//    int OPEN_MAX=sysconf(_SC_OPEN_MAX);
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in servaddr,cliaddr;

    listenfd=Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(5555);
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);

    Bind(listenfd,(SA *) &servaddr,sizeof(servaddr));
    Listen(listenfd,LISTENQ);

    client[0].fd=listenfd;
    client[0].events=POLLRDNORM;
    for(i=1;i<OPEN_MAX;i++)
        client[i].fd=-1;
     maxi=0;

    for(;;)
    {
        nready=poll(client,maxi+1,INFTIM);
        if(client[0].revents&POLLRDNORM)
        {
            len=sizeof(cliaddr);
            connfd=Accept(listenfd,(SA *)&cliaddr,&len);

            for(i=1;i<OPEN_MAX;i++)
                if(client[i].fd<0)
                {
                    client[i].fd=connfd;
                    break;
                }
            if(i==OPEN_MAX)
                err_quit("too many client");
            client[i].events=POLLRDNORM;
            if(i>maxi)
                maxi=i;
            if(--nready<=0)
                continue;

        }

        for(i=1;i<=maxi;i++)
        {
            if((sockfd=client[i].fd)<0)
                continue;
            if(client[i].revents&(POLLRDNORM|POLLERR))
            {
                if((n=read(sockfd,buf,MAXLINE))<0)
                {
                    if(errno==ECONNRESET)
                    {
                        Close(sockfd);
                        client[i].fd=-1;
                    }
                    else
                        err_sys("read error");

                }
                else if(n==0)
                {
                    Close(sockfd);
                    client[i].fd=-1;
                }
                else
                    Write(sockfd,buf,n);

                if(--nready<=0)
                    break;
            }
        }


    }
}
