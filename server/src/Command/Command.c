#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#include "Command.h"

#define MSGSIZE 3000

static char *substring(char *string, int position, int length);
static char** createListOfCommands(char* substringOfCommands, char* commandName);
static char* extractNameFromCommand(char* command);
char** tokenize(char* command);

char inbuf[MSGSIZE];

struct CommandStruct {
    char* command;
};

Command* newCommand(char* commandString) {
    Command* command = malloc(sizeof(Command));
    command->command = commandString;
    return command;
}

char** tokenize(char* command){
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

  char** args = malloc((sizeof(char*) * numberOfTokens) + 1);
  char* token = strtok (command ," ");
  int counter = 0;
  while (token != NULL)
  {
    strcpy((args+counter), token);
    token = strtok(NULL, " ");
    counter++;
  }
  counter++;
  *(args+counter) = NULL;
  printf("%s \n", args);
  return args;
}

char* execute(Command* command) {
  int fds[2];
  if(command->command[0] == 10) {
    return command->command;
  }
  //const char **argv = {"/bin/sh", "-c", command->command};

  pipe(fds);
  fcntl(fds[0], F_SETFL, O_NONBLOCK); /*Non Blocking Pipe */
  if (fork() == 0) {
      char** args = tokenize(command->command);
      printf("token 1 %s \n", args);
      printf("token 2 %s \n", args+1);
      char* commands[3];
      printf("asdfasdf \n");
      strcpy(commands[0], args);
      printf("asdfasdf \n");
      strcpy(commands[1], args+1);
      printf("asdfasdf \n");
      commands[2] = NULL;
      close(STDOUT_FILENO);
      close(fds[0]);
      dup2(fds[1], STDOUT_FILENO);
      dup2(fds[0], STDIN_FILENO);
      close(fds[1]);
      fflush(stdout);
      printf("asdf %s\n", commands[0]);
      printf("asdfas %s\n", commands[1]);
      execvp(commands[0], commands);
      perror("execvp failed");
      exit(0);
  }
  wait(0);
  char* msg = malloc(sizeof(char) * 3000);
  read(fds[0], msg, MSGSIZE);
  close(fds[1]);
  close(fds[0]);
  printf("Done Reading %s\n", msg);
  return msg;
}

int freeCommand(Command* command) {
    free(command);
    return 0;
}

static char *substring(char *string, int position, int length) {
   char dest[length];
   int len = strlen(string);
   int x = position;

   while (x*4 < len) {
       strncpy(dest, string+(x*4), 4);
       x++;
   }
   return dest;
}
