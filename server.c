#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

void err(int i, char*message){
  if(i < 0){
    printf("Error: %s - %s\n",message, strerror(errno));
    exit(1);
  }
}

int main(){
  printf("Setup time\n");
  struct addrinfo *hints, *results;
  hints = calloc(1,sizeof(struct addrinfo));
  char *port = "4242";
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socke
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo("127.0.0.1", port, hints, &results);
  //main socket creation
  int listener = socket(results->ai_family, results->ai_socktype, results->ai_protocol);//store the socket descriptor here
  err(listener,"make socket");
  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt error");
  //bind socket
  int binder = bind(listener, results->ai_addr, results->ai_addrlen);
  err(binder,"binding socket to address and port");

  listen(listener, 10);
  printf("ready to listen\n");

  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);
  /*int client_socket = accept(listener,(struct sockaddr *)&client_address, &sock_size);
  err(client_socket,"acceptor failure");*/

  fd_set read_fds;
  char buff[1025]="";
  int lineup[10]; //stores player socket codes
  int i = 0;
  int tournament = 0; //variable for when game is in session
  while(1){

    FD_ZERO(&read_fds);
    FD_SET(listener,&read_fds);
    int x = select(listener+1, &read_fds, NULL, NULL, NULL);
    err(x,"select");

    // if socket
    if (FD_ISSET(listener, &read_fds)) {
      //accept the connection
      if(!tournament){
        int client_socket = accept(listener,(struct sockaddr *)&client_address, &sock_size);
        err(client_socket,"acceptor failure");
        lineup[i]=client_socket; //store client
        char wutName[200];
        sprintf(wutName,"Hello, you are Player %d. Are we waiting on more players? (use lowercase)",i+1);
        int gimmeName = send(client_socket, wutName, strlen(wutName)+1, 0);
        err(gimmeName,"initial message");

        //read the whole buff
        int ro = read(client_socket,buff,sizeof(buff));
        err(ro,"recieving response to more players question");
        printf("Player %d confirmed\n",i+1);

        //trim the string
        buff[strlen(buff)-1]=0; //clear newline
        if(buff[strlen(buff)-1]==13){
          //clear windows line ending
          buff[strlen(buff)-1]=0;
        }
        if(strcmp(buff,"no")){
          tournament = 1;
        }

        //printf("\nPlayer Name: '%s'\n",buff);
        i++;
      }
      if(tournament){

      }
      //close(client_socket);
    }
  }

  free(hints);
  freeaddrinfo(results);

  return 0;
}
