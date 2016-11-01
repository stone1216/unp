#include "unp.h"

int tcp_connect(const char *host,const char *serv)
{
    int sockfd,n;
    struct addrinfo hints,*res,*aip;

    bzero(&hints,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    if((n=getaddrinfo(host,serv,&hints,&res))!=0)
        err_quit("getaddrinfo error",gai_strerror(n));

    for(aip=res;aip!=NULL;aip=aip->ai_next)
   {
        sockfd=socket(aip->ai_family,aip->ai_socktype,aip->ai_protocol);
        if(sockfd<0)
            continue;
        if(connect(sockfd,aip->ai_addr,aip->ai_addrlen)==0)
            break;
        Close(sockfd);
    }

    if(aip==NULL)
        err_sys("tcp_connect error for %s %s",host,serv);

    freeaddrinfo(res);

    return sockfd;

}
