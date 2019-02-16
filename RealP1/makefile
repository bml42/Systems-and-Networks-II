cflags = -g -Wall
CC = gcc

all: httpServer httpClient

serverObjects : httpServer.o serverFunctions.o

clientObjects : httpClient.o clientFunctions.o

httpServer : $(serverObjects)
	$(CC) $(cflags) serverFunctions.c httpServer.c -o httpServer

httpClient : $(clientObjects)
	$(CC) $(cflags) clientFunctions.c httpClient.c -o httpClient

httpServer.o : httpServer.c 

serverFunctions.o : serverFunctions.c serverFunctions.h

httpClient.o : httpClient.c

clientFunctions.o : clientFunctions.c clientFunctions.h

.PHONY : clean

clean :
	rm $(objects) httpServer httpClient
	
