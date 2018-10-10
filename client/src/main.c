#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./Socket/Socket.h"

int main() {
    char message[3000];
    char portMessage[6];
    char ipMessage[20];
    printf("Please Enter The IP Address you would like to connect to? \n");
    fgets(ipMessage, sizeof ipMessage, stdin);
    printf("Please Enter The Port You would like to connect to? \n");
    fgets(portMessage, sizeof portMessage, stdin);
    setupSocket(ipMessage, atoi(portMessage));
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
