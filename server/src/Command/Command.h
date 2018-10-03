#ifndef CommandHeader
# define CommandHeader

typedef struct CommandStruct Command;
Command* newCommand(char* commandString);
int freeCommand(Command* command);
char* execute(Command* command);

#endif
