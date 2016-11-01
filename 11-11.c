#include"unp.h"

int main(int argc,char **argv)
{
    int sockfd,n;
    char recvline[MAXLINE+1];
    socklen_t len;
    struct sockaddr_storage ss;
    if(argc!=3)
        err_quit("input argments error");

    sockfd=tcp_connect(argv[1],argv[2]);

    len=sizeof(ss);
    //getsockname(sockfd,(SA *)&ss,&len);
    getpeername(sockfd,(SA *)&ss,&len);
    printf("connect to %s\n",sock_ntop((SA *)&ss,len));

    while((n=read(sockfd,recvline,MAXLINE)))
    {
        recvline[n]=0;
        fputs(recvline,stdout);
    }
    exit(0);


}
