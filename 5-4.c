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



int main54(int argc, char *argv[])
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

    str_cli(fd);
    exit(0);



}
