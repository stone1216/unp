#include"unp.h"

int udp_server(const char *host,const char *serv,socklen_t *addrlenp)
{
    int sockfd,n;
    struct addrinfo hints,*res,*aiptr;
    bzero(&hints,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_flags=AI_PASSIVE;
    hints.ai_socktype=SOCK_DGRAM;

    if((n=getaddrinfo(host,serv,&hints,&res))!=0)
        err_quit("udp_server error for %s %s: %s",host,serv,gai_strerror(n));
    aiptr=res;
    for(;aiptr!=NULL;aiptr=aiptr->ai_next)
    {
        sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
        if(sockfd<0)
            continue;
        if(bind(sockfd,res->ai_addr,res->ai_addrlen)==0)
            break;
        else
            printf("%s",strerror(errno));
        Close(sockfd);
    }

    if(aiptr==NULL)
        err_quit("udp_server error for %s %s",host,serv);
    if(addrlenp)
        *addrlenp=res->ai_addrlen;
    freeaddrinfo(res);
    return sockfd;

}
