#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./Socket/Socket.h"

/* Port number to connect to and max message size */
#define PORT 5171
#define MSGSIZE 3000

int main() {
    char message[MSGSIZE];
    char ipMessage[20];
    printf("Please Enter The IP Address you would like to connect to? \n");
    fgets(ipMessage, sizeof(ipMessage), stdin);
    setupSocket(ipMessage, PORT);
    int initialServerResponse = 0;
    /* loops untill the user enters quit or uses ctrl-c */
    while(1) {
      if(!initialServerResponse) {
        /* Reads a message to indicate that there was an established connection */
        readFromSocket();
        initialServerResponse = 1;
      }
      printf("shell> ");
      /* Reads message from stdin to message string */
      fgets(message, sizeof message, stdin);
      if(!strcmp(message, "quit\n")) {
        exit(0);
      }
      /* Sends the message as long as its not empty */
      if(message[0] != 10) {
        sendMessage(message);
        readFromSocket();
      }
    }
    return 0;
}
