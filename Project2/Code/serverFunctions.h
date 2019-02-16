/*Token Ring serverFunctions.h file
*Programmers:  Ali Al-Senan
*              Benjamin Linam
*Code developed on April 15, 2018
*FOR: COP4635 Systems & Networks 2
*/


#ifndef SERVERFUNCTIONS
#define SERVERFUNCTIONS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>

extern int s;                           //socket descriptor
extern struct sockaddr_in servAddr;     //server socket structure

//receiveStrings_outgoingMail
//
//Parameter 1: int, number of expected peers
//Returns: nothing
void receiveStrings_outgoingMail(int);	
//receives connection messages from peers and sends each peer information about its neighbor
#endif
