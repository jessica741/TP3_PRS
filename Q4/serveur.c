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

FILE* Init_fichier_exchange (const char* Nomfichier );

int main (int argc, char *argv[]) {


  const char * NameFile = "testserveur.txt"; 
  struct sockaddr_in adresse, client;
  int port= atoi(argv[1]);
  int valid= 1;
  socklen_t alen= sizeof(client);
  char buffer[RCVSIZE];
  FILE * Echange_octet ; 
  //create socket
  int desc= socket(AF_INET, SOCK_DGRAM, 0);//UDP
  Echange_octet= Init_fichier_exchange (NameFile);

  

   
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
  
  //RCV SYN
  memset(buffer,0,RCVSIZE);
  int err= recvfrom(desc,buffer,sizeof(buffer), 0 , (struct sockaddr*)&adresse,&alen);
  fprintf(Echange_octet, "r %d ", err);
  if (  err == -1 )
  {
    printf("pb\n");
  }

  if (strcmp (buffer,"SYN") == 0 ) 
  {
    printf("Reception SYN (serveur) \n");
    int oct= sendto( desc, "SYNACK",6, 0, (struct sockaddr*) &adresse, (socklen_t)sizeof(adresse)  );
    fprintf(Echange_octet, "e %d ", oct);
    printf("Envoi SYNACK (serveur) \n");
  }
  memset(buffer,0,RCVSIZE);
  recvfrom(desc,buffer,sizeof(buffer), 0 , (struct sockaddr*)&adresse, &alen);
  if (strcmp(buffer,"ACK")==0 ) 
  {
    printf("Reception ACK (serveur)\n");
  }

  fclose(Echange_octet);
  while (1) {

      int msgSize= recvfrom(desc,buffer,sizeof(buffer), 0 , (struct sockaddr*)&adresse, &alen);
      printf("msg size : %d !\n",msgSize);
      if( msgSize <= 0 )
      {
          perror( "recvfrom() error \n" );
          return -1;
      }
      while (msgSize > 0) {
          printf("Je suis dans le fils !!!\n");
          sendto( desc, buffer,msgSize, 0,(struct sockaddr*) &adresse, (socklen_t)sizeof(adresse) );
  
          printf("%s",buffer);
          memset(buffer,0,RCVSIZE);
          msgSize= recvfrom(desc,buffer,sizeof(buffer), 0 , (struct sockaddr*)&adresse,(socklen_t*)sizeof(adresse));
      
      }
         exit(0); 

  }

close(desc);
return 0;
}


FILE* Init_fichier_exchange (const char* Nomfichier )
  {
    FILE* Echange_octet = NULL;
    Echange_octet = fopen(Nomfichier, "w");//"testserveur.txt"
    Echange_octet = fopen(Nomfichier, "a");
    
    if (Echange_octet != NULL)
    {
          
          fputs("Serveur ", Echange_octet);
          return Echange_octet; 
    }
    else
    {
          // On affiche un message d'erreur si on veut
          printf("Impossible d'ouvrir le fichier test.txt");
    }
  }