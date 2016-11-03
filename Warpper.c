#include "unp.h"



ssize_t  Readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr=vptr;
    nleft=n;
    while(nleft>0)
    {
        if((nread=read(fd,ptr,n))<0)
        {
            if(errno==EINTR)
                nread=0;
            else
                return -1;
        }
        else if(nread==0)
            break;
        nleft-=nread;
        ptr+=nread;
    }

    return (n-nleft);
}



static int read_cnt;
static char *read_ptr;
static char read_buf[MAXLINE];
static ssize_t my_read(int fd,char *ptr)
{
    if(read_cnt<=0)
     {
       again:
        if((read_cnt=read(fd,read_buf,sizeof(read_buf)))<0)
        {
            if(errno==EINTR)
                goto again;
            return -1;
        }else if(read_cnt==0)
            return 0;
        read_ptr=read_buf;
    }

    read_cnt--;
    *ptr=*read_ptr++;
    return 1;
}

ssize_t  Readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n,rc;
    char c,*ptr;
    ptr=vptr;

    for(n=1;n<maxlen;n++)
    {
        if((rc=my_read(fd,&c))==1)
        {
            *ptr++=c;
            if(c=='\n')
                break;
        }
        else if(rc==0)
        {
            *ptr=0;
            return (n-1);
        }
        else
            return -1;
    }

    *ptr=0;
    return n;


}
ssize_t  Writen(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwrite;
    const char *ptr;

    ptr=vptr;
    nleft=n;

    while(nleft>0)
    {
        if((nwrite=write(fd,ptr,n))<=0)
        {
            if(nwrite<0&&errno==EINTR)
                nwrite=0;
            else
                return -1;
        }

        nleft-=nwrite;
        ptr+=nwrite;
    }

    return n;

}

//socket wrapper funtion
int Socket(int domain, int type, int protocol)
{
    int n;
    if((n=socket( domain,  type,  protocol))<0)
        err_sys("socket error");

    return n;


}
void Bind(int sockfd, const struct sockaddr *addr,
                socklen_t addrlen)
{
    if(bind(sockfd,addr,addrlen)<0)
        err_sys("bind error");

}
void Listen(int sockfd, int backlog)
{
    char *ptr;
    if((ptr=getenv("LISTENQ"))!=NULL)
        backlog=atoi(ptr);
    if(listen(sockfd,backlog)<0)
        err_sys("listen error");

}
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int n;
    again:
    if((n=accept(sockfd,addr,addrlen))<0)
        if(errno==EINTR)
            goto again;
         else
            err_sys("accept error");
    return n;
}
void Connect(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen)
{
    if(connect(sockfd,addr,addrlen)<0)
        err_sys("connect error");
}

void  Close(int fd)
{
    if(close(fd)<0)
        err_sys("close error");
}

void  Write(int fd, void *buf, size_t n)
{
    int nwrite;
    again:
       if((nwrite=write(fd,buf,n))<0)
          if(errno==EINTR)
              goto again;
          else
              err_sys("write error %s",strerror(errno));
}

ssize_t  Read(int fd, void *buf, size_t n)
{
    int nread;
    again:
    if((nread=read(fd,buf,n))<0)
        if(errno==EINTR)
            goto again;
        else
            err_sys("read error %s",strerror(errno));
    return nread;
}


int  Select(int nfds, fd_set * readfds, fd_set *writefds, fd_set *excpfds, struct timeval *timeout)
{
    int n;
    again:
    if((n=select(nfds,readfds,writefds,excpfds,timeout))<0)
        if(errno==EINTR)
            goto again;
        else
        err_quit("select error");
    return n;
}

void Shutdown(int sockfd, int how)
{
    if(shutdown(sockfd,how)<0)
        err_quit("shutdown error : %s",strerror(errno));
}

int Inet_pton(int af, const char *src, void *dst)
{
    int n;
    if((n=inet_pton( af,src, dst))==0)
        err_sys("%s is not a right",src);
    else if(n<0)
        err_sys("%s is not a right",dst);
}


ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen)
{
    int n;
    if((n=sendto( sockfd,buf,  len,  flags, dest_addr,  addrlen))<0)
    {
        err_sys("sendto error %s",strerror(errno));
    }
    else
        return n;
}

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen)
{
    int n;
    if((n=recvfrom( sockfd,buf,  len,  flags, src_addr,  addrlen))<0)
    {
        err_sys("sendto error %s",strerror(errno));
    }
    else
        return n;
}


const char *sock_ntop(const struct sockaddr *sa,
                             socklen_t salen)
{
    static char str[128];
    char buf[8];
    struct sockaddr_in *sin;
    switch(sa->sa_family)
    {
    case AF_INET:
        sin=(struct sockaddr_in *)sa;
        if((inet_ntop(AF_INET,&sin->sin_addr,str,sizeof(str)))==NULL)
            return NULL;
        if(ntohs(sin->sin_port)!=0)
        {
            snprintf(buf,sizeof(buf),":%d",ntohs(sin->sin_port));
            strcat(str,buf);
        }
        return str;
    default:
        err_sys("not implement");
    }

}

