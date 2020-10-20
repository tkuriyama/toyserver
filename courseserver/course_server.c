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
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
  hints.ai_socktype = SOCK_STREAM;
  // hints.ai_flags = AI_PASSIVE;  
  getaddrinfo("127.0.0.1", "3490", &hints, &res);

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd < 0) 
    error("ERROR opening socket");  
  if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
    error("ERROR binding socket");
  printf("Started server on 127.0.0.1:3490\n");
  
  if (listen(sockfd, 20) < 0)
    error("ERROR listen failed");
  addr_size = sizeof client_addr;
  newfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);

  // print client info
  char ipstr[INET6_ADDRSTRLEN];
  int port;
  
  if (getpeername(newfd, (struct sockaddr *)&client_addr, &addr_size) < 0) {
    error("ERROR getpeername failed");
  }
  else {
    struct sockaddr_in *s = (struct sockaddr_in *)&client_addr;
    port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
    printf("Client IP address: %s\n", ipstr);
    printf("Client port      : %d\n", port);    
  }
  
  if (recv(newfd, &buf, BUFSIZE, 0) > 0) {
    if (send(newfd, &buf, BUFSIZE, 0) > 0) 
      printf("Echoed message: %s\n", buf);
  }
  else {
    printf("Recv returned 0, client closed connection.\n");
  }
  
  close(sockfd);
  close(newfd);
  freeaddrinfo(res);
  printf("Bye.\n");
}

