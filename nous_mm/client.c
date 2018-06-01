#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main () 
{
    int idSocket ;
    char prenom[10]="hello";//"a","m","a","u","r","y"
    char hey[10]="hey";
    struct sockaddr_in s_addr;
    char message[10];
    memset((char*)&s_addr,0,sizeof(s_addr));
    s_addr.sin_family=AF_INET;
    s_addr.sin_port=htons(1234);
    //inet_aton("127.0.0.1",&s_addr.sin_addr.s_addr); // INADDR_ANY INADDR_LOOPBACK
    //s_addr.sin_addr.s_addr=init_addr("127.0.0.1");
    s_addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    //printf("s_addr : %d\n",s_addr.sin_addr.s_addr);

    idSocket=socket(AF_INET,SOCK_STREAM,0);
    if (idSocket<0){
        perror("Ouverture Socket");
    }
    else
    {
        printf("Id Socket : %d et ADDR : %d \n",idSocket,s_addr.sin_addr.s_addr);
        int connection = connect(idSocket,(struct sockaddr *)&s_addr,sizeof(s_addr));
        printf("ça va %d \n",connection);
        printf("prenom %s\n",prenom);
        write(idSocket,prenom,10);
        write(idSocket,hey,10);
        read(idSocket,message,10);
        printf("msg : %s\n",message);
    }
}