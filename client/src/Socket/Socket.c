#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#include "Socket.h"

int removeSpaces(char *str);

struct sockaddr_in address;
int valread;
int sock;
struct sockaddr_in serv_addr;
char buffer[1024] = {0};

int sendMessage(char* message) {
  int length = strlen(message);
  write(sock, message, length);
  return 0;
}

int readFromSocket() {
  char size[10];
  if(recv(sock, size, 10, 0) == -1) {
    printf("recv error: %s \n", strerror(errno));
  }
  char* message = malloc(sizeof(char) * atoi(size));
  if(!message) {
      printf("%s\n", "Malloc failed 33 socket.c ");
  }
  if(recv(sock, message, atoi(size), 0) == -1) {
    printf("recv error: %s \n", strerror(errno));
  }
  printf("shell> %s \n", message);
  fflush(stdout);
  strcpy(message, "");
  free(message);
  return 0;
}

int setupSocket(char* ipMessage , int port) {
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
      printf("\n Socket creation error \n");
      return -1;
  }
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  if(strcmp("localhost", ipMessage)) {
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
      return -1;
  }
  printf("Connected \n");
  return 0;
}

int removeSpaces(char *str) {
    int count = 0;
    int i;
    for (i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i];
    str[count] = '\0';
    return 0;
}
