#ifndef CommandHeader
# define CommandHeard

typedef struct CommandStruct Command;
Command* newCommand(char* commandString);
int freeCommand(Command* command);
int execute(Command* command);

#endif
