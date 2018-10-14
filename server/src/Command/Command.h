#ifndef CommandHeader
# define CommandHeader

/* All of the following are public methods that should be accesable outside Command.c */
typedef struct CommandStruct Command;
Command* newCommand(char* commandString, int messageSize);
int freeCommand(Command* command);
int execute(Command* command, char* msg);

#endif
