#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define RCVSIZE 1024

int main (int argc, char *argv[]) {

  printf("argv[0] %s et argc %d\n",argv[0],argc);
  struct sockaddr_in adresse;
  int port= atoi(argv[2]);
  int valid= 1;
  char msg[RCVSIZE];
  char blanmsg[RCVSIZE];
  socklen_t client_size_2;
  //create socket
  int desc= socket(AF_INET, SOCK_DGRAM, 0);

  // handle error
  if (desc < 0) {
    perror("cannot create socket\n");
    return -1;
  }

  setsockopt(desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(port);
  inet_aton(argv[1] , adresse.sin_addr.s_addr);  //INADDR_LOOPBACK

  // connect
  //int rc= connect(desc, (struct sockaddr*)&adresse, sizeof(adresse));/////////////////////////////////////

 /* if (rc < 0) {
    perror("connect failed\n");
    return -1;
  }*/
  char syn[3]= "SYN";
  printf("11\n");
  sendto( desc, syn,strlen(syn), 0, (struct sockaddr*) &adresse, (socklen_t)sizeof(adresse)  );
  printf("22\n");
  recvfrom(desc,blanmsg,sizeof(blanmsg), 0 , (struct sockaddr*)&adresse, &client_size_2);
  printf("33\n");
  if (blanmsg == "SYN-ACK" ) 
  {
    sendto( desc, "ACK",3, 0, (struct sockaddr*) &adresse, (socklen_t)sizeof(adresse)  );
  }
  //recvfrom(desc,blanmsg,sizeof(blanmsg), 0 , (struct sockaddr*)&adresse, &client_size_2);
  
  printf("client2 SYN ACK OK \n");
  int cont= 1;
  while (cont) {

    /*
    switch (blanmsg)
    {
      case "SYN-ACK":
        msg="SYN";

      case "SYN" :

      default :
        memset(blanmsg,0,RCVSIZE);
        fgets(msg, RCVSIZE, stdin);
    }
*/
    fgets(msg, RCVSIZE, stdin);
    sendto( desc, msg,strlen(msg), 0, (struct sockaddr*) &adresse, (socklen_t)sizeof(adresse)  );
    recvfrom(desc,blanmsg,sizeof(blanmsg), 0 , (struct sockaddr*)&adresse, (socklen_t*)sizeof(adresse));

    printf("%s",blanmsg);


    
    if (strcmp(msg,"stop\n") == 0) {
      cont= 0; 
    }
  }

close(desc);
return 0;
}