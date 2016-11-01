#include "unp.h"


void dg_cli(FILE *fp,int sockfd,const SA *seraddr,socklen_t serlen)
{

    int n;
    char sendline[MAXLINE],recvline[MAXLINE+1];

    while(fgets(sendline,MAXLINE,fp)!=NULL)
    {
        Sendto(sockfd,sendline,strlen(sendline),0,seraddr,serlen);
        n=Recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
        recvline[n]=0;
        fputs(recvline,stdout);
    }
}

int main87(int argc,char *argv[])
{
    int sockfd;
    struct sockaddr_in seraddr;

    if(argc!=2)
        err_quit("intput argments error");

    bzero(&seraddr,sizeof(seraddr));
    seraddr.sin_family=AF_INET;
    seraddr.sin_port=htons(5555);
    Inet_pton(AF_INET,argv[1],&seraddr.sin_addr);

    sockfd=Socket(AF_INET,SOCK_DGRAM,0);
    dg_cli(stdin,sockfd,(SA *)&seraddr,sizeof(seraddr));
    exit(0);


}
