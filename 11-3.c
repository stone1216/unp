#include"unp.h"


int main113(int argc,char **argv)
{
    char *ptr,**pptr;
    char str[INET_ADDRSTRLEN];

    struct hostent *hptr;

    while(--argc)
    {
        ptr=*++argv;
        if((hptr=gethostbyname(ptr))==NULL)
        {
            err_msg("gethostbyname error for host : %s: %s",
                    ptr,hstrerror(h_errno));
            continue;

        }

        printf("official hostname : %s\n",hptr->h_name);

        for(pptr=hptr->h_aliases;*pptr!=NULL;pptr++)
            printf("\talias : %s\n",*pptr);

        switch(hptr->h_addrtype)
        {
           case AF_INET:
            pptr=hptr->h_addr_list;
            for(;*pptr!=NULL;pptr++)
                printf("\tip address:%s\n",inet_ntop(AF_INET,*pptr,str,sizeof(str)));
            break;
        default:
            printf("unkonw address types\n");
            break;


        }



    }
    exit(0);


}
