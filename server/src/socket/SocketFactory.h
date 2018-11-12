#ifndef SOCKETFACTORY
#define SOCKETFACTORY
#include "../Command/Command.h"


int TRUE;
int FALSE;

int readSignal;
int writeSignal;

extern int cacheSize;
extern int numberOfCommandsInCache;
extern char* listOfProcessesString;
extern char* messageFromProcess;
extern int* messageToRead;
extern int* writing;
extern int* reading;
extern Command* commandCache[10];

int socketInit(int messageSize, int portNumber);
int handleConnections();

#endif
