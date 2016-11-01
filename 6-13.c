#include"unp.h"

static void str_cli1(FILE *fp,int sockfd)
{
    char  buf[MAXLINE];
    int n;

    int maxfdp1,stdineof;
    fd_set rset;

    FD_ZERO(&rset);
    stdineof=0;
    for(;;)
    {
        if(stdineof==0)
            FD_SET(fileno(fp),&rset);
        FD_SET(sockfd,&rset);
        maxfdp1=max(sockfd,fileno(fp))+1;
        Select(maxfdp1,&rset,NULL,NULL,NULL);

        if(FD_ISSET(sockfd,&rset))
        {
            if((n=read(sockfd,buf,MAXLINE))==0)
            {
                if(stdineof==1)
                    return ;
                else
                    err_quit("server terminated");
            }
            write(STDOUT_FILENO, buf,n);
        }
        if(FD_ISSET(fileno(fp),&rset))
        {
            if((n=read(fileno(fp),buf,MAXLINE))==0)
            {
                stdineof=1;
                Shutdown(sockfd,SHUT_WR);
                FD_CLR(fileno(fp),&rset);
                continue;
            }
            Writen(sockfd,buf,n);
        }


    }
}



int main613(int argc, char *argv[])
{
    int fd;
    struct sockaddr_in seraddr;

    if(argc!=2)
        err_quit("intput argments error");

    fd=Socket(AF_INET,SOCK_STREAM,0);
    bzero(&seraddr,sizeof(seraddr));
    seraddr.sin_family=AF_INET;
    seraddr.sin_port=htons(5555);
    inet_pton(AF_INET,argv[1],&seraddr.sin_addr);

    Connect(fd,(SA *)&seraddr,sizeof(seraddr));

    str_cli1(stdin,fd);
    exit(0);



}
