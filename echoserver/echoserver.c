#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

#define BUFSIZE 1024

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main() {
  
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sockfd, newfd;

  char buf[BUFSIZE];
  
  // setup address
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
  hints.ai_socktype = SOCK_STREAM;
  // hints.ai_flags = AI_PASSIVE;     
  getaddrinfo("127.0.0.1", "3490", &hints, &res);

  // setup socket nad bind
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd < 0) 
    error("ERROR opening socket");

  if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
    error("ERROR binding socket");

  // listen and accept
  if (listen(sockfd, 20) < 0)
    error("ERROR listen failed");

  addr_size = sizeof client_addr;
  newfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);

  // echo client
  if (recv(newfd, &buf, BUFSIZE, 0) > 0) {
    if (send(newfd, &buf, BUFSIZE, 0) > 0) {
      printf("Echoed message: %s\n", buf);
    }
  }
  else {
    printf("Recv returned 0, client closed connection.\n");
  }

  // cleanup
  close(sockfd);
  close(newfd);
  freeaddrinfo(res);
  printf("Bye.\n");
}

