#include <unistd.h>
#include <stdio.h>

#include "socket/SocketFactory.h"

#define MSGSIZE 3000
#define PORTNUMBER 5165

int main(int argc, char const *argv[])
{
    socketInit(MSGSIZE, PORTNUMBER);
    handleConnections();
    return 0;
}
