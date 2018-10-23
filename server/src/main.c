#include <unistd.h>
#include <stdio.h>

#include "socket/SocketFactory.h"

/*Port number and max message size */
#define MSGSIZE 3000
#define PORTNUMBER 5171

int main(int argc, char const *argv[])
{
    /* Inits the socket */
    socketInit(MSGSIZE, PORTNUMBER);
    /* handles connections */
    handleConnections();
    return 0;
}
