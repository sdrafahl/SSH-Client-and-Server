#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "../Command/Command.h";

int sockfd, newsockfd, portno, clilen;
int link[2];
char pipeBuffer[4096];
struct sockaddr_in serv_addr, cli_addr;

int socketInit() {

   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   if(pipe(link)==-1) {
       exit(EXIT_FAILURE);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 5001;

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
            perror("ERROR on accept");
            exit(1);
        }
        if(!fork()) {
            dup2 (link[1], STDOUT_FILENO);
            close(link[0]);
            close(link[1]);
            close(sockfd);
            char socketBuffer[4096];
            bzero(socketBuffer, 4096);
            read(newsockfd, socketBuffer, 4095);
            Command* command = newCommand(socketBuffer);
            execute(command);
            freeCommand(command);
            exit();
        } else {
            close(newsockfd);
            close(link[1]);
            int bytes = read(link[0],pipeBuffer, sizeof(pipeBuffer));
            printf("Output: (%.*s)\n", bytes, pipeBuffer);
            wait(NULL);
        }
    }
}
