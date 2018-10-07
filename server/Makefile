csrc = $(wildcard src/*.c) \
       $(wildcard src/Command/*.c) \
	   $(wildcard src/socket/*.c) \

LDFLAGS = -Wall -ggdb -g

SSH:
	reset
	gcc $(LDFLAGS) $(csrc) -o sshServer
