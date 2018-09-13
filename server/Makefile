csrc = $(wildcard src/*.c) \
       $(wildcard src/Command/*.c) \
	   $(wildcard src/socket/*.c) \

LDFLAGS = -Wall -ggdb

SSH:
	reset
	gcc $(LDFLAGS) $(csrc) -o sshServer
