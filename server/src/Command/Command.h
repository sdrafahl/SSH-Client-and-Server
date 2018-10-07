#ifndef CommandHeader
# define CommandHeader

typedef struct CommandStruct Command;
Command* newCommand(char* commandString);
int freeCommand(Command* command);
int execute(Command* command, char* msg);

#endif
