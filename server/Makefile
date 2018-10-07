csrc = $(wildcard src/*.c) \
       $(wildcard src/command/*.c) \
	   $(wildcard src/socket/*.c) \

LDFLAGS = -Wall -ggdb -g

SSH:
	reset
	gcc $(LDFLAGS) $(csrc) -o sshServer
