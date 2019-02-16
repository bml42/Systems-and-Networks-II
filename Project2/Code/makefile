cflags = -g -Wall -pthread
CC = gcc

all: bbserver bbpeer

objects : serverObjects peerObjects

serverObjects : bbserver.o serverFunctions.o

peerObjects : bbpeer.o peerFunctions.o

bbserver : $(serverObjects)
	$(CC) $(cflags) bbserver.c serverFunctions.c -o bbserver

bbpeer	: $(peerObjects)
	$(CC) $(cflags) bbpeer.c peerFunctions.c -o bbpeer

bbserver.o : bbserver.c serverFunctions.h

bbpeer.o : bbpeer.c peerFunctions.h

peerFunctions.o : peerFunctions.c peerFunctions.h

serverFunctions.o : serverFunctions.c serverFunctions.h
.PHONY : clean

clean : 
	rm $(objects)  bbserver bbpeer
