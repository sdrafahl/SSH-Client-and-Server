#ifndef Command
# define Command

typedef struct CommandStruct Command;
Command* newCommand(unsigned int port);
int freeCommand(*Command command);

#endif
