#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <poll.h>

#include "../command/Command.h"
/* The socket, the socket of the client,port number, and the msg max size */
int sockfd, newsockfd, portno, msgSize;
/* socket struct */
socklen_t clilen;
/* socket address structs */
struct sockaddr_in serv_addr, cli_addr;

int probeSocket(int socket, char* message);

/* Initalizes the socket connection on the server side */
int socketInit(int messageSize, int portNumber) {

   portno = portNumber;
   msgSize = messageSize;
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }

   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   return 0;
}

/* Gets connections from clients and spins off new processes for each one */
int handleConnections() {
    while(1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0) {
            printf("Error Connecting");
            perror("ERROR on accept \n");
            exit(1);
        }
        int pid = fork();

        if(pid < 0) {
          perror("ERROR on fork \n");
          exit(1);
        }

        if(pid == 0) {
            int sentConnectedMessage = 0;
            struct pollfd pfd;
            pfd.fd = newsockfd;
          	pfd.events = POLLIN | POLLHUP | POLLRDNORM;
          	pfd.revents = 0;
            close(sockfd);
            char message[3000];
            if(!sentConnectedMessage) {
              strcpy(message, "Connected to server....");
              sentConnectedMessage = 1;
            }
            probeSocket(newsockfd, message);
            while(1) {
              /* Checks if there is data */
              if(poll(&pfd, 1, 100) > 0) {
                char socketBuffer[3000];
                bzero(socketBuffer, 3000);
                /* reads incoming command */
                if(recv(newsockfd, &socketBuffer, 3000, MSG_DONTWAIT) == 0) {
                  printf("Socket Closed \n");
                  exit(0);
                }

                if(socketBuffer[0] != '\n') {
                  Command* command = newCommand(socketBuffer, msgSize);
                  char stdoutFromExec[msgSize];
                  execute(command, stdoutFromExec);
                  probeSocket(newsockfd, stdoutFromExec);
                  freeCommand(command);
                }
              }
            }
        } else {
            close(newsockfd);
        }
    }
}

/* sends a message to the client, else it determines if the socket is no longer connected to disconnect it */
int probeSocket(int socket, char* message) {
  if(message[0] == '\0') {
    strcpy(message, "Command Complete ");
  }
  int length = strlen(message);
  char numberString[10];
  sprintf(numberString, " %d", length);
  send(socket, numberString, 10, 0);
  if(send(socket, message, strlen(message), 0) >= 0) {
    return 0;
  }
  return 1;
}
