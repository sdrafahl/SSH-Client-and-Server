#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../Command/Command.h"

int sockfd, newsockfd, portno;
socklen_t clilen;
int link_[2];
char pipeBuffer[4096];
struct sockaddr_in serv_addr, cli_addr;

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
   portno = 8062;

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
            close(sockfd);
            char socketBuffer[4096];
            bzero(socketBuffer, 4096);
            read(newsockfd, socketBuffer, 4095);
            Command* command = newCommand(socketBuffer);
            execute(command);
            freeCommand(command);
        } else {
            close(newsockfd);
        }
    }
}
