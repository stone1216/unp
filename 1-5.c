
#include"unp.h"

#define MAXLINE 2
int main15(int argc,char *argv[])
{
    int sockfd,n;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;

    if(argc!=2)
        err_quit("input argments error");
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
        err_sys("socket error");

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(5555);
    if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0)
        err_quit("inet_pton error");

    if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr))<0)
        err_sys("connect error");

    while((n=read(sockfd,recvline,MAXLINE))>0)
    {
        //recvline[n]=0;
       // if(fputs(recvline,stdout)==EOF)
       //     err_sys("fputs error");
        
        
        if(write(STDOUT_FILENO,recvline,n)<0)
            err_sys("write error");

    }
    if(n<0)
        err_sys("read error");
    exit(0);




    return 0;
}

