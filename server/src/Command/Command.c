#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#include "Command.h"

static char *substring(char *string, int position, int length);
static char** createListOfCommands(char* substringOfCommands, char* commandName);
static char* extractNameFromCommand(char* command);
char* ExecuteAndReturnString(const char *command, const char *type);
char** tokenize(char* command);

struct CommandStruct {
    char* command;
};

Command* newCommand(char* commandString) {
    Command* command = malloc(sizeof(Command));
    command->command = commandString;
    return command;
}

char* ExecuteAndReturnString(const char *command, const char *type) {
    int fds[2];
    if(command[0] == 10) {
      return command;
    }
    const char *argv[4] = {"/bin/sh", "-c", command};
    pipe(fds);
    printf("%s\n", command);
    fcntl(fds[0], F_SETFL, O_NONBLOCK); /*Non Blocking Pipe */
    if (fork() == 0) {
        close(STDOUT_FILENO);
        dup(fds[1]);
        close(fds[0]);
        close(fds[1]);
        execvp(argv[0], argv);
        perror("execvp failed");
        exit(0);
    }
    wait(0);
    char msg[3000];
    printf("Begin Reading \n");
    read(fds[0], msg, 3000);
    printf("Done Reading %s\n", msg);
    close(fds[0]);
    close(fds[1]);
    return msg;
}

char* execute(Command* command) {

    /*  char** commands = malloc(sizeof(char) * strlen(command->command));

      int memmoryCounter = 0;
      int x;
      int firstLetter;
      int firstLetterSet;
      for(x=0;x<strlen(command->command); x++){
        if(*(command->command+x) =! ' ') {
          if(!firstLetterSet) {
            firstLetter = x;
            firstLetterSet = 1;
          } else {
            strcpy((commands+memmoryCounter), substring(command->command, firstLetter, x-firstLetter));
            printf("command: %s \n", (commands+memmoryCounter));
            memmoryCounter++;
            firstLetterSet = 0;
          }
        }
      }*/
      char* message = ExecuteAndReturnString(command->command, "r");
      return message;
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
