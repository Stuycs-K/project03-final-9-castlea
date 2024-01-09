#include "connections.h"
#include "rps.h"
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

int server_setup() { //returns socket descriptor
  //setup structs for getaddrinfo
  struct addrinfo *hints, *results;
  char *port = "4242";
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socke
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo("127.0.0.1", port, hints, &results);
  //create the socket
  int listener = socket(results->ai_family, results->ai_socktype, results->ai_protocol);//store the socket descriptor here
  err(listener,"make socket in server_setup");
  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");

  //bind the socket to address and port
  int binder = bind(listener, results->ai_addr, results->ai_addrlen);
  err(binder,"binding socket to address and port");
  //set socket to listen state
  listen(listener, 10);

  //free the structs used by getaddrinfo
  freeaddrinfo(hints);
  freeaddrinfo(results);

  return listener;
}

int client_connection(){
  struct addrinfo *hints, *results;
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  getaddrinfo(("127.0.0.1","4242",hints,&results);
  int connector = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  err(connector,"connector socket setup");
  connect(connector, results->ai_addr, results->ai_addrlen);
  freeaddrinfo(hints);
  freeaddrinfo(results);
  return connector;
}
