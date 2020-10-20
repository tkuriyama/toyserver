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

/* Network Operations */

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

/* Database helpers */

void ewrap(int status, char *name) {
  if (status == 0)
    printf("> Error: %s process returned error code\n", name);
  else 
    printf("> Success: %s\n", name); 
}

void ewrapResponse(int status, char **msg) {
  if (status == 0)
    strcpy(*msg, "Error: process returned error code\n");
  else 
    strcpy(*msg, "Success\n");
}

/* Database Operations */

int process_request(FILE *fd, char *buf, char *response) {
  int sent = 0; //set to 1 if fd stream is used to send to client
  
  if (strcmp(buf, "init\n") == 0) {
    if (db.initialized) 
      strcpy(response, "DB already initialized\n");
    else
      ewrapResponse(init_database(), &response);
  }
  else if (strcmp(buf, "stats\n") == 0) {
    if (db.initialized) {
      print_stats_sock(fd);
      sent = 1;
      printf("Sent: (stats retrieved and sent).\n\n");
    }
    else {
      strcpy(response, "DB is closed.\n");
    }
  }
  else if (strcmp(buf, "load\n") == 0) {
      ewrapResponse(load_tables("TestIO"), &response);
  }
  else if (strcmp(buf, "close\n") == 0) {
    ewrapResponse(clear_database(), &response);
  }
  else {
    strcpy(response, "Command not recognized.\n");
  }
  
  return sent;
}

/* Main */

int main() {
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
  
  struct sockaddr_storage client_addr;
  socklen_t addr_size;

  addr_size = sizeof client_addr;
  newfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
  print_client_info(newfd, &client_addr, &addr_size);

  FILE *clientfd = fdopen(newfd, "w");
  while (1) {
    int sent = 0;
    
    if (recv(newfd, &buf, BUFSIZE, 0) > 0) {
      if (strcmp(buf, "bye\n") == 0) {
	printf("> Client asked to close connection.\n");
	break;
      }
      else {
	printf("Received: %s", buf);
	sent = process_request(clientfd, buf, response);
      }
      
      if (!sent) { 
	if (send(newfd, &response, BUFSIZE, 0) > 0) 
	  printf("Sent: %s\n", response);
	else
	  error("> ERROR failed to send");
      }
      
      memset(buf, 0, strlen(buf));
      memset(response, 0, strlen(response));
    }
    else {
      printf("> Recv returned 0, client closed connection.\n");
      break;
    }
  }

  ewrap(clear_database(), "Closing DB");
  close(sockfd);
  fclose(clientfd);   // this closes clientfd and also fd
  freeaddrinfo(res);
  printf("> Bye.\n");
}

