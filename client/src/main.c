#include <unistd.h>
#include <stdio.h>

#include "Socket/Socket.h"

int main(int argc, char const *argv[])
{
    char message[3000];
    char portMessage[6];
    char ipMessage[20];
    printf("Please Enter The IP Address you would like to connect to? \n");
    fgets(ipMessage, sizeof ipMessage, stdin);
    printf("Please Enter The Port You would like to connect to? \n");
    fgets(portMessage, sizeof portMessage, stdin);
    setupSocket(ipMessage , atoi(portMessage));
    int initialServerResponse = 0;
    while(1) {
      if(!initialServerResponse) {
        readFromSocket();
        initialServerResponse = 1;
      }
      printf("shell>");
      fgets(message, sizeof message, stdin);
      if(strcmp(message, "quit")) {
        exit(0);
      }
      sendMessage(message);
      readFromSocket();
    }
}
