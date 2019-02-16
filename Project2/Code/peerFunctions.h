/*Token Ring peerFunctions.h file
*Programmers:  Ali Al-Senan
*              Benjamin Linam
*Code developed on April 15, 2018
*FOR: COP4635 Systems & Networks 2
*/

#ifndef PEERFUNCTIONS
#define PEERFUNCTIONS

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#define messagelen 10000

//Global Variables
extern int numOfHosts;
extern int token;
extern int s;
extern int optionEdited;
extern int start;
extern char option;
extern const char delimiter[3];
extern pthread_mutex_t lock;
extern struct sockaddr_in neighborAddr;

//inputThread
//
//Parameter 1: char*, Name of the file being used by the peer.
//Return: None
void *inputThread(void *a);

//setConnection
//
//Parameter 1: struct hostent, used to aquire the IP of the connection.
//Parameter 2: int, used to send in the port number of the connection.
//Return: struct sockaddr_in, the neighbors address structure.
struct sockaddr_in setConnection(struct hostent *name,int port); 

//firstContact
//
//Parameter 1: struct sockaddr_in*, sends in the host information.
//Parameter 2: struct hostent, used to aquire the IP of the connection.
//Parameter 3: int, used to send in the port number of the connection.
//Parameter 4: char*, send in main argument 5 to check whether it's joining an existing ring.
//Return: struct sockaddr_in, the neighbors address structure.
struct sockaddr_in firstContact(struct sockaddr_in *host,struct hostent *name,int port,char *arg);

//loopingCommunication
//
//Parameter 1: char*, send in main argument 5 to check whether it's joining an existing ring.
//Parameter 2: char*, Name of the file being used by the peer.
//Return: None
void loopingCommunication(char *arg,char* fileName);

#endif
