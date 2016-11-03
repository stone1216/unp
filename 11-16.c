#include"unp.h"

int main1116(int argc,int **argv)
{
    int sockfd,n;
    char recvline[MAXLINE+1];
    socklen_t salen;
    struct sockaddr *sa;

    if(argc!=3)
        err_quit("input argments error");


    sockfd=udp_client(argv[1],argv[2],(void **)&sa,&salen);
    printf("sending to %s\n",sock_ntop(sa,salen));
    sendto(sockfd," ",1,0,sa,salen);
    n=recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
    recvline[n]='\0';
    fputs(recvline,stdout);
    exit(0);
}
