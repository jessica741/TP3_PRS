#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>

#define RCVSIZE 1024

int main (int argc, char *argv[]) {

  struct sockaddr_in adresse, client;
  int port2= atoi(argv[1]);
  int valid= 1;
  socklen_t alen= sizeof(client);
  char buffer[RCVSIZE];

  //create socket
  int desc2= socket(AF_INET, SOCK_DGRAM, 0);//UDP

  // handle error
  if (desc2 < 0) {
    perror("cannot create socket\n");
    return -1;
  }
  // envois paquet Ã  udpaddr
  /*if( sendto( desc2, buffer, //taille msg //, 0, &adresse, sizeof(adresse) ) == SOCKET_ERROR )
    {
        printf( "sendto() error %d\n", SOCKET_ERRNO );
        return -1;
    }*/

  setsockopt(desc2, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(port2);
  adresse.sin_addr.s_addr= htonl(INADDR_ANY);

  if (bind(desc2, (struct sockaddr*) &adresse, sizeof(adresse)) == -1) {
    perror("Bind fail\n");
    close(desc2);
    return -1;
  }
  
  //RCV SYN
  memset(buffer,0,RCVSIZE);
  int err= recvfrom(desc2,buffer,sizeof(buffer), 0 , (struct sockaddr*)&adresse,&alen);
  if (  err ==-1 )
  {
    printf("pb\n");
  }

  printf("coucou1 %s\n",buffer);
  if (strcmp (buffer,"SYN") == 0 ) 
  {
    printf("ugrek\n");
    printf("coucou2 %s\n",buffer);
    sendto( desc2, "SYNACK",6, 0, (struct sockaddr*) &adresse, (socklen_t)sizeof(adresse)  );
  }
  memset(buffer,0,RCVSIZE);
  recvfrom(desc2,buffer,sizeof(buffer), 0 , (struct sockaddr*)&adresse, &alen);
  if (strcmp(buffer,"ACK")==0 ) 
  {
    printf("serveur ok\n");
  }


  while (1) {


    //fd_set descripteurs;
    //FD_ZERO(&descripteurs);
    //FD_SET(desc2,&descripteurs);
    //select (4,&descripteurs , NULL , NULL , NULL );
    /*int pid= fork () ;
    if ( pid==-1)
    {
      // il y a une erreur 
      perror("une erreur s'est produite ! \n");
      exit(-1);
    }*/
    //else if ( pid == 0)
    //{
      // je suis dans le fil
      //printf("desc2 : %d !\n",desc2);
      
      //close(desc2); // je me fiche de regarder qui se connecte 
      int msgSize= recvfrom(desc2,buffer,sizeof(buffer), 0 , (struct sockaddr*)&adresse, &alen);
      printf("msg size : %d !\n",msgSize);
      if( msgSize <= 0 )
      {
          perror( "recvfrom() error \n" );
          return -1;
      }
      while (msgSize > 0) {
          printf("Je suis dans le fils !!!\n");
          sendto( desc2, buffer,msgSize, 0,(struct sockaddr*) &adresse, (socklen_t)sizeof(adresse) );
  
          printf("%s",buffer);
          memset(buffer,0,RCVSIZE);
          msgSize= recvfrom(desc2,buffer,sizeof(buffer), 0 , (struct sockaddr*)&adresse,(socklen_t*)sizeof(adresse));
       //  (struct sockaddr*)&adresse,(socklen_t*)sizeof(adresse)
      }
         exit(0); // utile si msgSize== stop par exemple cond arret 
         // la fonction exit prend tjs un paramÃ¨tre on met 0 pour Ã©viter les erreurs
    //}
    //else 
    //{
    // je suis dans lepere
    //  printf(" le fils %d\n",pid );
      

    //}
      

  }

close(desc2);
return 0;
}
