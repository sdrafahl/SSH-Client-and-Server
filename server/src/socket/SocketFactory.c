#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <poll.h>

#include "../Command/Command.h"

int sockfd, newsockfd, portno;
socklen_t clilen;
int link_[2];
char pipeBuffer[4096];
struct sockaddr_in serv_addr, cli_addr;

int probeSocket(int socket, char* message);

int socketInit() {

   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   if(pipe(link_)==-1) {
       exit(EXIT_FAILURE);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 9010;

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
            struct pollfd pfd;
            pfd.fd = newsockfd;
          	pfd.events = POLLIN | POLLHUP | POLLRDNORM;
          	pfd.revents = 0;
            close(sockfd);
            char message[3000];
            strcpy(message, "Connected to server....");
            probeSocket(newsockfd, message);
            while(1) {
              if(poll(&pfd, 1, 100) > 0) {
                char socketBuffer[3000];
                bzero(socketBuffer, 3000);
                if(recv(newsockfd, &socketBuffer, 3000, MSG_DONTWAIT) == 0) {
                  printf("Socket Closed \n");
                  exit(0);
                }
                Command* command = newCommand(socketBuffer);
                strcpy(message, execute(command));
                probeSocket(newsockfd, message);
                freeCommand(command);
              }
            }
        } else {
            close(newsockfd);
        }
    }
}

int probeSocket(int socket, char* message) {
  if(message[0] == '\0') {
    strcpy(message, "Command Complete");
  }
  if(send(socket, message, strlen(message), 0) >= 0) {
    return 0;
  }
  return 1;
}
