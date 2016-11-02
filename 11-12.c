#include"unp.h"


int tcp_listen(const char *host,const char *serv,socklen_t *addrlen)
{
    int listenfd, n;
    struct addrinfo hints,*res,*aiptr;
    const int on=1;

    bzero(&hints,sizeof(hints));
    hints.ai_flags=AI_PASSIVE;
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;

    if((n=getaddrinfo(host,serv,&hints,&res))!=0)
        err_quit("getaddrinfo error %s",gai_strerror(n));


    for(aiptr=res;aiptr!=NULL;aiptr=aiptr->ai_next)
    {
        listenfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
        if(listenfd<0)
            continue;
        setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
        if(bind(listenfd,res->ai_addr,res->ai_addrlen)==0)
            break;
        Close(listenfd);
    }

    if(aiptr==NULL)
        err_sys("tcp_listen error for %s %s",host,serv);
    Listen(listenfd,LISTENQ);

    if(addrlen)
        *addrlen=res->ai_addrlen;
    freeaddrinfo(res);

    return listenfd;


}
