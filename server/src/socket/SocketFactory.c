#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <poll.h>
#include <signal.h>
#include <fcntl.h>

#include "./SocketFactory.h"
#include "../Command/Command.h"
#include "../encrypt/encrypt.h"

/* The socket, the socket of the client,port number, and the msg max size */
int sockfd, newsockfd, portno, msgSize;
/* socket struct */
socklen_t clilen;
/* socket address structs */
struct sockaddr_in serv_addr, cli_addr;

int probeSocket(int socket, char* message);
void* createSharedMemory(size_t size);

char** listOfProcesses; // list of commands
int numberOfProcesses = 0;
int lengthOfProcesses = 100;

char* listOfProcessesString;
char* messageFromProcess;
int* messageToRead;
int* writing;
int* reading;

int cacheSize = 10;
Command* commandCache[10];
int numberOfCommandsInCache;

/* Initalizes the socket connection on the server side */
int socketInit(int messageSize, int portNumber) {

   TRUE = 1;
   FALSE = 0;
   listOfProcesses = malloc(100 * sizeof(char*));
   int y;
   for(y=0;y<lengthOfProcesses;y++) {
       listOfProcesses[y] = "";
   }

   listOfProcessesString = (char*) createSharedMemory(sizeof(int*) * 20);
   reading = (int*) createSharedMemory(sizeof(int*));
   writing = (int*) createSharedMemory(sizeof(int*));
   messageToRead = (int*) createSharedMemory(sizeof(int*));
   messageFromProcess = (char*) createSharedMemory(sizeof(char*) * 20);
   memcpy(listOfProcessesString, "", sizeof(char));
   memcpy(reading, &FALSE, sizeof(int));
   memcpy(writing, &FALSE, sizeof(int));
   memcpy(messageToRead, &FALSE, sizeof(int));
   memcpy(messageFromProcess, "", sizeof(char));
   portno = portNumber;
   msgSize = messageSize;
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   int flags = fcntl(sockfd, F_GETFL);
   fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

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
        if(*messageToRead) {
            if(*writing) {
                wait(&writeSignal);
            }

            if(*reading) {
                wait(&readSignal);
            }

            memcpy(writing, &TRUE, sizeof(int));

            char* commands[3];
            tokenize(messageFromProcess, " ", commands);

            if(strcmp(commands[0],"ADD") == 0) {

                if(lengthOfProcesses == numberOfProcesses) {
                    listOfProcesses = realloc(listOfProcesses, lengthOfProcesses * lengthOfProcesses * sizeof(char*));
                    lengthOfProcesses = lengthOfProcesses * lengthOfProcesses;
                }

                listOfProcesses[numberOfProcesses] = malloc(strlen(commands[1]) * sizeof(char));
                strcpy(listOfProcesses[numberOfProcesses], commands[1]);
                numberOfProcesses++;
            }

            if(strcmp(commands[0],"KILL") == 0) {

                wait(NULL); /* reaps zombie process */
                int processToDeleteIndex;
                int x;
                for(x=0;x<numberOfProcesses;x++) {
                    if(strcmp(commands[1], listOfProcesses[x]) == 0) {
                        processToDeleteIndex = x;
                    }
                }
				free(listOfProcesses[processToDeleteIndex]);
                for(x=processToDeleteIndex;x<numberOfProcesses;x++) {
                    if(x == numberOfProcesses-1) {
                        listOfProcesses[x] = NULL;
                    } else {
                        char* point = listOfProcesses[x+1];
                        listOfProcesses[x] = point;
                    }
                }
                strcpy(messageFromProcess, "");
                numberOfProcesses--;
            }

            strcpy(listOfProcessesString, "");
            int a;
            for(a=0;a<numberOfProcesses;a++) {
                strcat(listOfProcessesString, listOfProcesses[a]);
                if(a+1<numberOfProcesses) {
                    strcat(listOfProcessesString, "\n");
                }
            }
            memcpy(writing, &FALSE, sizeof(int));
			memcpy(messageToRead, &FALSE, sizeof(int));
            signal(writeSignal, NULL);
        }

        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if(newsockfd > 0) {
          int pid = fork();

          if(pid < 0) {
            perror("ERROR on fork \n");
            exit(1);
          }

          if(pid == 0) {

    	      if(*reading) {
                  wait(&readSignal);
              }

              if(*writing) {
                  wait(&writeSignal);
              }

              while(*messageToRead) {
                  sleep(1);
              }

              numberOfCommandsInCache = 0;
              memcpy(writing, &TRUE, sizeof(int));
              char command[50];
              strcpy(command, "ADD ");
  			  int pid = getpid();
  			  sprintf(command, "ADD %i", pid);
              strcpy(messageFromProcess, command);

              memcpy(writing, &FALSE, sizeof(int));
              memcpy(messageToRead, &TRUE, sizeof(int));
              signal(writeSignal, NULL);

              int sentConnectedMessage = 0;
              struct pollfd pfd;
              pfd.fd = newsockfd;
              pfd.events = POLLIN | POLLHUP | POLLRDNORM;
              pfd.revents = 0;
              close(sockfd);
              char message[3000];
              if(!sentConnectedMessage) {
                strcpy(message, "\n");
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
                    /* socket disconnected */
                    if(*reading) {
                        wait(&readSignal);
                    }

                    if(*writing) {
                        wait(&writeSignal);
                    }

                    while(*messageToRead) {
                        sleep(1);
                    }

                    memcpy(writing, &TRUE, sizeof(int));
                    char command[50];
  				    int pid = getpid();
                    sprintf(command, "KILL %i", pid);
  				    strcpy(messageFromProcess, command);
                    memcpy(writing, &FALSE, sizeof(int));
                    memcpy(messageToRead, &TRUE, sizeof(int));
                    signal(writeSignal, NULL);
                    printf("Socket Closed \n");
                    exit(0);
                  }

                  decryptSlide(socketBuffer);

                  if(socketBuffer[0] != '\n') {
                    int semiColons = 0;
                    int x;
                    for(x = 0;x<strlen(socketBuffer);x++) {
                        if(socketBuffer[x] == ';') {
                            semiColons++;
                        }
                    }
                    semiColons++;
                    char* messageToClient = malloc(msgSize * sizeof(char));
                    strcpy(messageToClient, "");
                    char* commands[semiColons];
                    tokenize(socketBuffer, ";", commands);

                    for(x = 0;x < semiColons;x++) {
                        Command* command = newCommand(commands[x], msgSize);
                        char stdoutFromExec[msgSize];
                        execute(command, stdoutFromExec);
                        strcat(messageToClient, stdoutFromExec);
                        if(x+1<semiColons) {
                            strcat(messageToClient, ",");
                        }
                        if(numberOfCommandsInCache > 9) {
                            freeCommand(commandCache[numberOfCommandsInCache % cacheSize]);
                        }
                        commandCache[numberOfCommandsInCache % cacheSize] = command;
                        numberOfCommandsInCache++;
                    }

                    probeSocket(newsockfd, messageToClient);
                    free(messageToClient);
                  }
                }
                sleep(1);
              }
          }
          close(newsockfd);
        }
        sleep(1);
    }
}

void* createSharedMemory(size_t size) {
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_ANONYMOUS | MAP_SHARED;
  return mmap(NULL, size, protection, visibility, -1, 0);
}

/* sends a message to the client, else it determines if the socket is no longer connected to disconnect it */
int probeSocket(int socket, char* message) {
  if(message[0] == '\0') {
    strcpy(message, "Command Complete ");
  }
  encryptSlide(message);
  int length = strlen(message);
  char numberString[10];
  sprintf(numberString, " %d", length);
  encryptSlide(numberString);
  send(socket, numberString, 10, 0);
  if(send(socket, message, strlen(message), 0) >= 0) {
    return 0;
  }
  return 1;
}
