#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "../socket/SocketFactory.h"
#include "./Command.h"
#include "../encrypt/encrypt.h"

/* Code in this file represents a command that is parsed and ready for execution. */

int numberOfTokens(char* command);

/* Command constructor */
Command* newCommand(char* commandString, int messageSize) {
    Command* command = malloc(sizeof(Command*) * 2);
    if(!command) {
        printf("%s\n", "malloc failed at line 23 Command.c ");
    }
    command->command = malloc(sizeof(char) * messageSize);
    strcpy(command->command, "");
    strcpy(command->command, commandString);
    command->messageSize = messageSize;
    return command;
}

/* Returns the number of tokens from the client -helper method */
int numberOfTokens(char* command) {
  int numberTokens = 1;
  int length = strlen(command);
  int countedSpace = 0;
  int x;
  for(x=0; x< length;x++) {
    if(command[x] == ' ' && !countedSpace) {
      numberTokens++;
      countedSpace = 1;
    } else if(command[x] != ' ') {
      countedSpace = 0;
    }
  }
  return numberTokens;
}

/* Removes the newline character at the end of the tokens -helper method  */
int removeNewLine(char* str) {
    int counter = 0;
    for(counter = 0;counter<strlen(str);counter++){
        if(str[counter] == 10) {
            str[counter] = '\0';
            break;
        }
    }
    return 0;
}

/* converts string from client into a set of tokens in args -helper method */
int tokenize(char* command, char** args) {
  char* token = strtok(command ," ");
  int counter = 0;
  while (token != NULL)
  {
    args[counter] = token;
    token = strtok(NULL, " ");
    counter++;
  }
  args[counter] = 0;
  return 0;
}
/* executes the command, this is a public method */
int execute(Command* command, char* msg) {
  int fds[2];
  int errorPipe[2];
  memset(msg, '\0', command->messageSize);
  if(command->command[0] == 10) {
    msg[0] = 0;
    return 0;
  }
  pipe(fds);
  pipe(errorPipe);
  fcntl(errorPipe[0], F_SETFL, O_NONBLOCK); /*Non Blocking Pipe */
  fcntl(fds[0], F_SETFL, O_NONBLOCK); /*Non Blocking Pipe */
  int tokens = numberOfTokens(command->command);
  char* args[tokens+1];
  tokenize(command->command, args);

  int x;
  for(x=0;x<tokens;x++) {
      removeNewLine(args[x]);
  }
  /* cd doesnt work correctly in execvp so I am using chdir and printing out the running directory using getcwd */
  if(strcmp(args[0], "cd") == 0){
      if(chdir(args[1]) != 0){
          printf("%s\n", "Error");
      }
      getcwd(msg, command->messageSize);
      return 0;
  } else if(strcmp(args[0], "jobs") == 0) {
      printf("%s\n", "command is a job");
      if(*writing) {
          wait(&writeSignal);
      }
      if(*reading) {
          wait(&readSignal);
      }

      memcpy(reading, &TRUE, sizeof(int));
      printf("%s\n", listOfProcessesString);
      write(fds[1], listOfProcessesString, strlen(listOfProcessesString) + 1);
      memcpy(reading, &FALSE, sizeof(int));
      signal(readSignal, NULL);
      read(fds[0], msg, command->messageSize);
      close(fds[1]);
      close(fds[0]);
      return 0;

  } else if(strcmp(args[0], "History") == 0) {
      int length = numberOfCommandsInCache;
      if(numberOfCommandsInCache > 9) {
        length = 10;
      }
      int size = 0;
      int x;
      for(x=0;x<length;x++) {
          size += commandCache[x]->messageSize;
      }
      char* historyString = malloc(size * sizeof(char));
      strcpy(historyString, "");
      printf("Length %i\n", length);
      for(x=0;x<length;x++) {
          printf("X %i\n", x);
          printf("Command Stored %s\n", commandCache[x]->command);
          printf("command read address %i\n", commandCache[x]);
          strcat(historyString, commandCache[x]->command);
          strcat(historyString, " \n ");
      }
      strcpy(msg, historyString);
      strcpy(historyString, "");
      free(historyString);
      close(fds[0]);
      close(fds[1]);
      return 0;
  } else {
      if (fork() == 0) {
          close(STDOUT_FILENO);
          close(fds[0]);
          dup2(fds[1], STDOUT_FILENO);
          dup2(fds[0], STDIN_FILENO);
          dup2(errorPipe[1], STDERR_FILENO);
          close(fds[1]);
          fflush(stdout);

          if(0 < execvp(args[0], args)) {
               perror("execvp failed");
               printf("Command Failed! \n");
          }
          exit(0);
      }
      wait(0);
      char errorMessage[3000];
      read(fds[0], msg, command->messageSize);
      read(errorPipe[0], errorMessage, command->messageSize);
      strcat(msg, errorMessage);
      close(errorPipe[1]);
      close(errorPipe[0]);
      close(fds[1]);
      close(fds[0]);
      return 0;
  }
}

/* Command deconstructor */
int freeCommand(Command* command) {
    free(command->command);
    free(command);
    return 0;
}
