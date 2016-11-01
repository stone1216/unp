#include"unp.h"


int main114(int argc,char **argv)
{
    struct hostent *hp;
    struct servent *sp;
    struct in_addr **pptr;
    struct sockaddr_in servaddr;
    char str[INET_ADDRSTRLEN];
    char recvline[MAXLINE+1];
    int sockfd,n;
    if(argc!=3)
        err_quit("input argments error");

    if((hp=gethostbyname(argv[1]))==NULL)
        err_quit("gethostbyname error\n");
    else
        pptr=(struct in_addr **)hp->h_addr_list;

    if((sp=getservbyname(argv[2],"tcp"))==NULL)
        err_quit("get servbyname error");

    for(;*pptr!=NULL;pptr++)
    {
        sockfd=Socket(AF_INET,SOCK_STREAM,0);
        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family=AF_INET;
        servaddr.sin_port=sp->s_port;



        memcpy(&servaddr.sin_addr,*pptr,sizeof(struct in_addr));


        printf("trying %s\n",sock_ntop(&servaddr,sizeof(servaddr)));

        if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr))==0)
            break;
        err_ret("connect error %s",strerror(errno));
        Close(sockfd);
    }

    if(*pptr==NULL)
      err_quit("unable to connect");

      while((n=Read(sockfd,recvline,MAXLINE))>0)
      {
          recvline[n]=0;
          fputs(recvline,stdout);
         // write(STDOUT_FILENO,recvline,n);
      }
      exit(0);


}
