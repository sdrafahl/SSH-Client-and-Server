#ifndef SOCKETFACTORY
#define SOCKETFACTORY

int TRUE;
int FALSE;

int readSignal;
int writeSignal;

char* listOfProcessesString;
char* messageFromProcess;
int* messageToRead;
int* writing;
int* reading;

int socketInit(int messageSize, int portNumber);
int handleConnections();

#endif
