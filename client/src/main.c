#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./Socket/Socket.h"

#define PORT 5164
#define MSGSIZE 3000

int main() {
    char message[MSGSIZE];
    char ipMessage[20];
    printf("Please Enter The IP Address you would like to connect to? \n");
    fgets(ipMessage, sizeof ipMessage, stdin);
    setupSocket(ipMessage, PORT);
    int initialServerResponse = 0;
    while(1) {
      if(!initialServerResponse) {
        readFromSocket();
        initialServerResponse = 1;
      }
      printf("shell> ");
      fgets(message, sizeof message, stdin);
      if(!strcmp(message, "quit\n")) {
        exit(0);
      }
      if(message[0] != 10) {
        sendMessage(message);
        readFromSocket();
      }
    }
    return 0;
}
