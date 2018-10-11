# RSH

This is a basic remote shell and server project. You are able to run commands via terminal to the terminal.

## Installation

### Server

To install the server once you have the project is to navigate into the server directory and run the make file and then run
the program sshServer.

```
cd server
make
./sshServer
```

### Client

To install the client once you have the project is to navigate into the client directory and run the make file and then run
the program sshServer.

```
cd client
make
./sshClient
```

## How To Use

1. Run the server

```
./sshServer
```

2. Run the client

```
./sshClient
```

3. Enter in the IP address when prompted or enter localhost for 127.0.0.1.

4. Enter in a command.

## Exiting The Program

On the client side you can enter
`quit`
or your can use ctrl+c.

The server will probe the client to see if the connection still remains so after using ctrl+c the client will no longer be reading commands and the connection will close.

After a socket is closed the server will print that a socket was closed.

## PORT 5164
The last four digits of my ID is 0164 but I made it 5164 so you dont have to run the program in kernel mode/sudo.
