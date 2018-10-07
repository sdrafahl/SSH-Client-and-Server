#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "Command.h"

#define MSGSIZE 3000

int tokenize(char* command, char** args);

int numberOfTokens(char* command);

char inbuf[MSGSIZE];

struct CommandStruct {
    char* command;
};

Command* newCommand(char* commandString) {
    Command* command = malloc(sizeof(Command*));
    if(!command) {
        printf("%s\n", "malloc failed at line 23 Command.c ");
    }
    command->command = commandString;
    return command;
}

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

int execute(Command* command, char* msg) {
  int fds[2];
  if(command->command[0] == 10) {
    msg[0] = 0;
    return 0;
  }
  pipe(fds);
  fcntl(fds[0], F_SETFL, O_NONBLOCK); /*Non Blocking Pipe */
  int tokens = numberOfTokens(command->command);
  char* args[tokens+1];
  tokenize(command->command, args);

  int x;
  for(x=0;x<tokens;x++) {
      removeNewLine(args[x]);
  }

  if(strcmp(args[0], "cd") == 0){
      printf("here we go %s\n ", args[1]);
      int x;
      for(x=0;x<strlen(args[1]);x++) {
        printf("%i\n ", args[1][x]);
      }
      if(chdir(args[1]) != 0){
          printf("%s\n", "Error");
      }
      getcwd(msg, MSGSIZE);
      return 0;
  } else {
      if (fork() == 0) {
          close(STDOUT_FILENO);
          close(fds[0]);
          dup2(fds[1], STDOUT_FILENO);
          dup2(fds[0], STDIN_FILENO);
          close(fds[1]);
          fflush(stdout);

          if(0 < execvp(args[0], args)) {
               perror("execvp failed");
               printf("Command Failed! \n");
          }
          exit(0);
      }
      wait(0);
      read(fds[0], msg, MSGSIZE);
      close(fds[1]);
      close(fds[0]);
      return 0;
  }
}

int freeCommand(Command* command) {
    free(command);
    return 0;
}
