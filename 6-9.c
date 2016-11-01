#include"unp.h"



static str_cli(int fd)
{
    char  sendline[MAXLINE],recvline[MAXLINE];
    int n;

    while((n=read(STDIN_FILENO,sendline,MAXLINE))>0)
    {
        Writen(fd,sendline,n);
        if(Readline(fd,recvline,MAXLINE)==0)
            err_quit("server terminate permaturely");

        write(STDOUT_FILENO,recvline,strlen(recvline));
    }
}

static str_cli1(int fd)
{
    char  sendline[MAXLINE],recvline[MAXLINE];
    int n;

    int maxfdp1;
    fd_set rset;
    FD_ZERO(&rset);
    for(;;)
    {
        FD_SET(0,&rset);
        FD_SET(fd,&rset);
        maxfdp1=fd+1;
        Select(maxfdp1,&rset,NULL,NULL,NULL);

        if(FD_ISSET(fd,&rset))
        {
            if((n=Readline(fd,recvline,MAXLINE))==0)
                err_quit("server terminated");
            write(STDOUT_FILENO, recvline,n);
        }
        if(FD_ISSET(0,&rset))
        {
            if((n=read(STDIN_FILENO,sendline,MAXLINE))==0)
                return ;
            Writen(fd,sendline,n);
        }


    }
}



int main69(int argc, char *argv[])
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

    str_cli1(fd);
    exit(0);



}
