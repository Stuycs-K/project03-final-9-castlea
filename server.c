#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

void err(int i, char*message){
  if(i < 0){
	  printf("Error: %s - %s\n",message, strerror(errno));
  	exit(1);
  }
}

int server_setup(){ //returns socket descriptor
  //getaddrinfo
  struct addrinfo *hints, *results;
  char *port = "4242";
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socke
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo("127.0.0.1", port, hints, &results);
  //main socket creation
  int listener = socket(results->ai_family, results->ai_socktype, results->ai_protocol);//store the socket descriptor here
  err(listener,"make socket in server_setup");
  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");
  //bind socket
  int binder = bind(listener, results->ai_addr, results->ai_addrlen);
  err(binder,"binding socket to address and port");
  //inlistening state
  return listener;
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
  int client_socket = accept(listener,(struct sockaddr *)&client_address, &sock_size);
  err(client_socket,"acceptor failure");

  free(hints);
  freeaddrinfo(results);

  return 0;
}
