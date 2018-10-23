#ifndef COMMAND
#define COMMAND

typedef struct {
    char* command;
    int messageSize;
} Command;

/* All of the following are public methods that should be accesable outside Command.c */
int tokenize(char* command, char** args);
Command* newCommand(char* commandString, int messageSize);
int freeCommand(Command* command);
int execute(Command* command, char* msg);

#endif
