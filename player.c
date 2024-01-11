#include "rps.h"
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

//int send(int sockfd, const void *msg, int len, int flags);
//sockfd is the socket descriptor you want to send data to (whether it’s the one returned by socket() or the one you got with accept()).
// is a pointer to the data you want to send, and len is the length of that data in bytes.
//set flags to 0

//int recv(int sockfd, void *buf, int len, int flags);
//sockfd is the socket descriptor to read from
// is the buffer to read the information into
// is the maximum length of the buffer, and flags can again be set to 0

//int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);
//sockfd is the descriptor of the connected stream socket
//addr is a pointer to a struct sockaddr (or a struct sockaddr_in) that will hold the information about the other side of the connection,
//addrlen is a pointer to an int, that should be initialized to sizeof *addr or sizeof(struct sockaddr).



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
  int accepted = client_connection();
  char buff[200];
  int rere = read(accepted,buff,sizeof(buff));
  err(rere,"recieving first message");
  printf("%s\n",buff);
  fgets(buff,sizeof(buff),stdin);
  int hereName = send(accepted,buff,sizeof(buff),0);


  return 0;
}
