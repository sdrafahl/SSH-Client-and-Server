csrc = $(wildcard src/*.c) \
       $(wildcard src/Socket/*.c) \
	   $(wildcard src/encrypt/*.c) \

LDFLAGS = -Wall -ggdb

SSH:
	gcc $(LDFLAGS) $(csrc) -o sshClient
