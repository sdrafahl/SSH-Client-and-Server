#include <unistd.h>
#include <stdio.h>

#include "Socket/Socket.h"

int main(int argc, char const *argv[])
{
    char message[100];
    while(1) {
      setupSocket();
      printf("Please Enter a command: \n");
      fgets(message, sizeof message, stdin);
      sendMessage(message);
    }
}
