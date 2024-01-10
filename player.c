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

int client_connection(){
  struct addrinfo *hints, *results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  getaddrinfo("127.0.0.1", "4242", hints, &results);
  int connector = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  err(connector,"connector socket setup");
  connect(connector, results->ai_addr, results->ai_addrlen);
  printf("connected\n");
  free(hints);
  freeaddrinfo(results);
  return connector;
}

int main(){
  printf("player\n");
  client_connection();
  return 0;
}
