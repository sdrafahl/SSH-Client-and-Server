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

char** tokenize(char* command, int* tokens);

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

char** tokenize(char* command, int* tokens){
  int numberOfTokens = 1;
  int length = strlen(command);
  int countedSpace = 0;
  int x;
  for(x=0; x< length;x++) {
    if(command[x] == ' ' && !countedSpace) {
      numberOfTokens++;
      countedSpace = 1;
    } else if(command[x] != ' ') {
      countedSpace = 0;
    }
  }
  char** args = malloc((sizeof(char) * (numberOfTokens + 1)));
  if(!args) {
      printf("%s\n", "malloc failed at line 44 Command.c ");
  }
  char* token = strtok(command ," ");
  int counter = 0;
  while (token != NULL)
  {
    args[counter] = token;
    token = strtok(NULL, " ");
    counter++;
  }
  counter++;
  args[counter] = NULL;
  *tokens = numberOfTokens;
  return args;
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

char* execute(Command* command) {
  int fds[2];
  if(command->command[0] == 10) {
    return command->command;
  }
  char msg[MSGSIZE];
  pipe(fds);
  fcntl(fds[0], F_SETFL, O_NONBLOCK); /*Non Blocking Pipe */
  int tokens;
  char** args = tokenize(command->command, &tokens);

  if(strcmp(args[0], "cd") == 0){
      if(chdir(args[2]) != 0){
          printf("%s\n", "Error");
      }
      getcwd(msg, 1024);
      //free(args);
      return msg;
  } else {
      if (fork() == 0) {
          printf("%s\n", "After ");
          close(STDOUT_FILENO);
          close(fds[0]);
          dup2(fds[1], STDOUT_FILENO);
          dup2(fds[0], STDIN_FILENO);
          close(fds[1]);
          fflush(stdout);

          int x;
          for(x=0;x<tokens;x++) {
              removeNewLine(args[x]);
          }

          if(0 < execvp(args[0], args)) {
               perror("execvp failed");
               printf("Command Failed! \n");
          }
          exit(0);
      }
      wait(0);
      //free(args);
      read(fds[0], msg, MSGSIZE);
      close(fds[1]);
      close(fds[0]);
      printf("Done Reading %s\n", msg);
      return msg;
  }
}

int freeCommand(Command* command) {
    free(command);
    return 0;
}
