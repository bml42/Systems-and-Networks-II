//clientFunctions.h-TCP Connection
//Programmers:	Ali Al-Senan
//		Benjamin Linam
//Code developed on March 5, 2018
//FOR: COP4635 Systems & Networks 2

#ifndef CLIENTFUNCTIONS_H_
#define CLIENTFUNCTIONS_H_


#include <stdio.h>          //Standard library
#include <stdlib.h>         //Standard library
#include <sys/socket.h>     //API and definitions for the sockets
#include <sys/types.h>      //more definitions
#include <netinet/in.h>     //Structures to store address information
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define IP_ADDR "143.88.64.158"
#define ARRLEN 300
#define LONGFILE 30000

void serverCommunication(int);

#endif /*TCPCLIENT3_H_*/
