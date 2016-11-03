#include "unp.h"

int main621()
{
    int listenfd,connfd,maxfd,i,maxi,sockfd;
    socklen_t len;

    char buf[MAXLINE];
    int client[FD_SETSIZE],nready,n;
    fd_set allset,rset;

    struct sockaddr_in servaddr,cliaddr;

    listenfd=Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(5555);
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);

    Bind(listenfd,(SA *) &servaddr,sizeof(servaddr));
    Listen(listenfd,LISTENQ);

    maxfd=listenfd;
    maxi=-1;

    for(i=0;i<FD_SETSIZE;i++)
        client[i]=-1;
    FD_ZERO(&allset);
    FD_SET(listenfd,&allset);

    for(;;)
    {
        rset=allset;
        nready=Select(maxfd+1,&rset,NULL,NULL,NULL);

        if(FD_ISSET(listenfd,&rset))
        {
            len=sizeof(cliaddr);
            connfd=Accept(listenfd,(SA *)&cliaddr,&len);

            for(i=0;i<FD_SETSIZE;i++)
                if(client[i]<0)
                {
                    client[i]=connfd;
                    break;
                }
            if(i==FD_SETSIZE)
                err_quit("too many client");
            FD_SET(connfd,&allset);
            if(connfd>maxfd)
                maxfd=connfd;
            if(i>maxi)
                maxi=i;

            if(--nready<=0)
                continue;

        }

        for(i=0;i<=maxi;i++)
        {
            if((sockfd=client[i])<0)
                continue;
            if(FD_ISSET(sockfd,&rset))
            {
                if((n=Read(sockfd,buf,MAXLINE))==0)
                {
                    Close(sockfd);
                    FD_CLR(sockfd,&allset);
                    client[i]=-1;

                }
                else
                    Write(sockfd,buf,n);

                if(--nready<=0)
                    break;
            }
        }


    }
}
