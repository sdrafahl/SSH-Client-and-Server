csrc = $(wildcard src/*.c) \
			 $(wildcard src/socket/*.c) \
       $(wildcard src/Command/*.c) \
	   	 $(wildcard src/encrypt/*.c) \

LDFLAGS = -Wall -ggdb -g

SSH:
	reset
	gcc $(LDFLAGS) $(csrc) -o sshServer
