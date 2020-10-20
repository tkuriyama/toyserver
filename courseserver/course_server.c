#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include "coursedb.c"
#include "coursedb.h"

#define BUFSIZE 1024

void error(char *msg) {
    perror(msg);
    exit(0);
}

void print_client_info(int newfd, struct sockaddr_storage *client_addr,
		       socklen_t *addr_size) {
  char ipstr[INET6_ADDRSTRLEN];
  int port;
  
  if (getpeername(newfd, (struct sockaddr *)client_addr, addr_size) < 0) {
    error("ERROR getpeername failed");
  }
  else {
    struct sockaddr_in *s = (struct sockaddr_in *)client_addr;
    port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
    printf("> Client connected from: %s:%d\n", ipstr, port);
  }
}

char *process_request(char *buf) {
  return buf;
}

int main() {
  
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sockfd, newfd;

  char buf[BUFSIZE];
  char response[BUFSIZE];

  char *ip = "127.0.0.1";
  char *port = "3490";
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(ip, port, &hints, &res);

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd < 0) 
    error("ERROR opening socket");  
  if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
    error("ERROR binding socket");
  printf("> Started server on %s:%s\n", ip, port);
    
  if (listen(sockfd, 20) < 0)
    error("ERROR listen failed");
  addr_size = sizeof client_addr;

  newfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
  print_client_info(newfd, &client_addr, &addr_size);
  
  while (1) {
    if (recv(newfd, &buf, BUFSIZE, 0) > 0) {
      if (strcmp(buf, "bye") == 0) {
	printf("> Client asked to close connection.");
      }
      else {
	printf("Received: %s", buf);
	strcpy(response, process_request(buf));
      }
      if (send(newfd, &response, BUFSIZE, 0) > 0) 
	printf("Sent: %s\n", response);
      else
	error("> ERROR failed to send");
    }
    else {
      printf("> Recv returned 0, client closed connection.\n");
      break;
    }
  }
  
  close(sockfd);
  close(newfd);
  freeaddrinfo(res);
  printf("> Bye.\n");
}

