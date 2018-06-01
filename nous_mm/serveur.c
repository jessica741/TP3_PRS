#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main () 
{
    int idSocket;
    int reuse = 1;
    int i = 0 ; 
    char buffer[10];
    char buffer2[10];
    struct sockaddr_in my_addr;
    struct sockaddr c_addr;
    socklen_t lenght = sizeof(my_addr);
    memset((char*)&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(1234);
    my_addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK); // INADDR_ANY INADDR_LOOPBACK

    //printf("ANY : %d, LOOPBACK : %d\n",INADDR_ANY,ntohl(INADDR_LOOPBACK));

    idSocket=socket(AF_INET,SOCK_STREAM,0);
    setsockopt(idSocket,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    if (idSocket<0){
        perror("Ouverture Socket");
    }
    else
        {
            //printf("Id Socket : %d et ADDR : %d\n",idSocket,my_addr.sin_addr.s_addr);
            int lien = bind(idSocket,(struct sockaddr *)&my_addr,sizeof(my_addr));
            int ecoute = listen(idSocket,10);
            //printf("Lien %d et Ecoute %d\n",lien, ecoute);
            while (1)
            {
                int accepter = accept(idSocket,(struct sockaddr *)&c_addr,&lenght);
                printf("Coucou %d\n",accepter);
                read(accepter,buffer,10);
                printf(" %s\n",buffer);
                 read(accepter,buffer2,10);
                printf(" %s\n",buffer2);
                write(accepter,buffer,10);

                printf("%s %d\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
            }
        }

}