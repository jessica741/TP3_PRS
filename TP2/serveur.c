#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>

#define RCVSIZE 1024

int main (int argc, char *argv[]) {

  struct sockaddr_in adresse, client;
  int port= atoi(argv[1]);

  int port2= atoi(argv[2]);
  int valid= 1;
  socklen_t alen= sizeof(client);
  char buffer[RCVSIZE];

  //create socket
  int desc= socket(AF_INET, SOCK_STREAM, 0);//TCP

  int desc2= socket(AF_INET, SOCK_DGRAM, 0);//UDP

  

  // handle error
  if (desc < 0) {
    perror("cannot create socket\n");
    return -1;
  }

  setsockopt(desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(port);
  adresse.sin_addr.s_addr= htonl(INADDR_ANY);

  if (bind(desc, (struct sockaddr*) &adresse, sizeof(adresse)) == -1) {
    perror("Bind fail\n");
    close(desc);
    return -1;
  }


  if (listen(desc, 5) < 0) {
    printf("Listen failed\n");
    return -1;
  }

  while (1) {


    fd_set descripteurs;
    FD_ZERO(&descripteurs);
    FD_SET(desc,&descripteurs);
    FD_SET(desc2,&descripteurs);

    


    int rqst= accept(desc, (struct sockaddr*)&client, &alen);
    int pid= fork () ;
    if ( pid==-1)
    {
      // il y a une erreur 
      printf("une erreur s'est produite ! \n");
    }
    else if ( pid == 0)
    {
      // je suis dans le fil
      printf("desc : %d /n",desc);
      printf("rqst : %d\n",rqst);
      close(desc); // je me fiche de regarder qui se connecte 
      int msgSize= recv(rqst,buffer,RCVSIZE,0);

        while (msgSize > 0) {
          printf("Je suis dans le fils !!!\n");
          
          send(rqst,buffer,msgSize,0);
          printf("%s",buffer);
          memset(buffer,0,RCVSIZE);
          msgSize= recv(rqst,buffer,RCVSIZE,0);
         
        }
         exit(0); // utile si msgSize== stop par exemple cond arret 
         // la fonction exit prend tjs un paramètre on met 0 pour éviter les erreurs
    }
    else 
    {
    // je suis dans lepere
      printf(" le fils %d\n",pid );
      
      close(rqst);

    }
      

  }

close(desc);
return 0;
}