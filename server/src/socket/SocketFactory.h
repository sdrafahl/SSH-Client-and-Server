#ifndef SOCKETFACTORY
#define SOCKETFACTORY

int TRUE;
int FALSE;

int readSignal;
int writeSignal;

extern char* listOfProcessesString;
extern char* messageFromProcess;
extern int* messageToRead;
extern int* writing;
extern int* reading;

int socketInit(int messageSize, int portNumber);
int handleConnections();

#endif
