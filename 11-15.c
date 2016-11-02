#include"unp.h"

int udp_client(const char *host,const char *serv,SA **saptr,socklen_t *lenp)
{
    int sockfd, n;
    struct addrinfo hints,*res,*aiptr;
    bzero(&hints,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_DGRAM;

    if((n=getaddrinfo(host,serv,&hints,&res))!=0)
        err_quit("udp_client error for %s %s",host,serv,gai_strerror(n));

    aiptr=res;

    for(;aiptr!=NULL;aiptr=aiptr->ai_next)
    {
        sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
        if(sockfd>=0)
            break;
        Close(sockfd);
    }

    if(res==NULL)
        err_sys("udp_client err for %s %s",host,serv);

    *saptr=malloc(res->ai_addrlen);
    memcpy(*saptr,res->ai_addr,res->ai_addrlen);
    *lenp=res->ai_addrlen;

    freeaddrinfo(res);

    return sockfd;


}
