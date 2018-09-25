#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "Command.h"

static char *substring(char *string, int position, int length);
static char** createListOfCommands(char* substringOfCommands, char* commandName);
static char* extractNameFromCommand(char* command);

struct CommandStruct {
    char* command;
};

Command* newCommand(char* commandString) {
    printf("Command String: %s", commandString);
    Command* command = malloc(sizeof(Command));
    int x;
    int lengthOfCommandString = strlen(commandString);
    for(x=0;x<lengthOfCommandString;x++) {
        if(commandString[x] == ' ') {
            command->command = commandString;
        }
    }
    return command;
}

char* execute(Command* command) {
      FILE* file = popen(command->command, "r");
      char buffer[100];
      while(fgets(buffer, 100, file)) {
        printf("%s", buffer);
      }
      return buffer;
}

int freeCommand(Command* command) {
    free(command);
    return 0;
}

static char *substring(char *string, int position, int length) {
   char *subString;
   int c;

   subString = malloc(length+1);

   for (c = 0 ; c < length ; c++) {
      *(subString+c) = *(string+position-1);
      string++;
   }

   *(subString+c) = '\0';

   return subString;
}

static char** createListOfCommands(char* substringOfCommands, char* commandName) {
    int countFlag = 0;
    char** flags = calloc(strlen(substringOfCommands)+1, sizeof(char*));
    int flagMemmoryIncramentor = 0;
    *flags = commandName;
    int x;
    for(x=1;x<strlen(substringOfCommands);x++) {
        if(substringOfCommands[x] == ' ' && !countFlag) {
            countFlag = 1;
            *(flags+flagMemmoryIncramentor) = substring(substringOfCommands, 1, x+1);
            flagMemmoryIncramentor++;
        }
        if(substringOfCommands[x] != ' ') {
            countFlag = 0;
        }
    }
    free(substringOfCommands);
    return flags;
}

static char* extractNameFromCommand(char* command) {
    int x;
    int lastForwardSlash = 0;
    for(x=0;x<strlen(command);x++) {
        if(command[x] == '/') {
            lastForwardSlash = x;
        }
    }
    return substring(command, lastForwardSlash+1, strlen(command)-(lastForwardSlash+1));
}
