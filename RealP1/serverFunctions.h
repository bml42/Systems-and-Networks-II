//serverFunctions.h-TCP Connection
//Programmers:	Ali Al-Senan
//		Benjamin Linam
//Code developed on March 5, 2018
//FOR: COP4635 Systems & Networks 2

#ifndef FUNCTIONS
#define FUNCTIONS

#include <stdio.h>          
#include <stdlib.h>         
#include <sys/socket.h>     
#include <sys/types.h>      
#include <netinet/in.h>     
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define MAXSIZE 1000000
#define BUFFER 12288 //12KB
#define CONTENT_LENGTH 10000
#define DELAY 250000

// communication
// param 1: Server Socket (int)
// param 2: Client Socket (int)
// param 3: Client Address (struct sockaddr*)
// param 4: Server Address (struct sockaddr*)
void communication(int,int,struct sockaddr*,struct sockaddr*);

// error
// param 1: Error Type (int)
void error(int);


#endif
