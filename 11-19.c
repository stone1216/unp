#include"unp.h"
#include<time.h>

int main(int argc,char **argv)
{
    int sockfd;
    ssize_t n;
    char buf[MAXLINE];
    time_t ticks;
    socklen_t len;
    struct sockaddr_storage cliaddr;

    if(argc==2)
        sockfd=udp_server(NULL,argv[1],NULL);
    else if(argc==3)
        sockfd=udp_server(argv[1],argv[2],NULL);
    else
        err_quit("input argments error");

    for(;;)
    {
        len=sizeof(cliaddr);
        n=recvfrom(sockfd,buf,MAXLINE,0,(SA *)&cliaddr,&len);
        printf("datagram from %s\n",sock_ntop((SA *)&cliaddr,len));

        ticks=time(NULL);
        snprintf(buf,MAXLINE,"%.24s\r\n",ctime(&ticks));
        sendto(sockfd,buf,MAXLINE,0,(SA *)&cliaddr,len);

    }
}
