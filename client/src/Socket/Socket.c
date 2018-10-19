#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#include "Socket.h"
#include "../encrypt/encrypt.h"

int removeSpaces(char *str);

struct sockaddr_in address;
int valread;
int sock;
struct sockaddr_in serv_addr;
char buffer[1024] = {0};

/* Sends a message to the server */
int sendMessage(char* message) {
  int length = strlen(message);
  encrypt(message);
  write(sock, message, length);
  return 0;
}

/* reads from socket */
int readFromSocket() {
  char size[10];
  /* First reads the size of the incoming message */
  if(recv(sock, size, 10, 0) == -1) {
    printf("recv error: %s \n", strerror(errno));
  }
  decrypt(size);
  char* message = malloc(sizeof(char) * atoi(size));
  if(!message) {
      printf("%s\n", "Malloc failed 33 socket.c ");
  }
  /* Secondly reads the message */
  if(recv(sock, message, atoi(size), 0) == -1) {
    printf("recv error: %s \n", strerror(errno));
  }
  decrypt(message);
  printf("shell> %s \n", message);
  fflush(stdout);
  strcpy(message, "");
  free(message);
  return 0;
}

/* Initalizes socket and connects to server */
int setupSocket(char* ipMessage , int port) {
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
      printf("\n Socket creation error \n");
      return -1;
  }
  /* Clear any memmory just in case */
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  if(strcmp("localhost", ipMessage) == 0) {
    strcpy(ipMessage, "127.0.0.1");
  }
  removeSpaces(ipMessage);

  if(inet_pton(AF_INET, ipMessage, &serv_addr.sin_addr)<=0)
  {
      printf("\nInvalid address/ Address not supported \n");
      return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
      printf("\nConnection Failed \n");
      exit(0);
      return -1;
  }
  printf("Connected \n");
  return 0;
}

/* Removes any extra spaces in a string */
int removeSpaces(char *str) {
    str[strlen(str) - 1] = '\0';
    return 0;
}
