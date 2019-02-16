/*Token Ring serverFunctions.c file
 *Programmers:	Ali Al-Senan
 *		Benjamin Linam
 *Code developed on April 15, 2018
 *FOR: COP4635 Systems & Networks 2
 */
#include"serverFunctions.h"

int s;//socket descriptor
struct sockaddr_in servAddr;//server socket structure used in bbserver.c main

void receiveStrings_outgoingMail(int numPeer)//function takes in an integer value for the number of peers connecting to server
{
        int n;
	socklen_t slen;					//length of message received 
        char buffer[246] = {32};            		//data buffer stored with ascii space char
        struct sockaddr_in clntListAddr[numPeer];	//array of peer socket addresses 
	struct sockaddr_in clntAddr;			//peer socket address

        for(n=0;n<numPeer;++n)  //run for number of expected peers
        {
                slen = sizeof(struct sockaddr_in);	
                recvfrom(s,buffer,sizeof(buffer),0,(struct sockaddr*)&clntAddr,&slen);	//receive inital connection message from numPeer peers
                clntListAddr[n] = clntAddr;						//array of socket informatin for each peer that sends message to server 
                printf("\nHost %d Connected\nIP: %s\nPort: %u\n",n+1,inet_ntoa(clntListAddr[n].sin_addr),clntListAddr[n].sin_port);//output message showing info on connected peers
        }
        for(n=0;n<numPeer;++n)//loop will send each peer the informatino of their neighbor (next client in array sorted by connection order)
        {
                memset(buffer,0,strlen(buffer));					//clear message buffer after every sendto
                if(n==numPeer-1)
                        sprintf(buffer,"%s:%u:1:%d",inet_ntoa(clntListAddr[0].sin_addr),clntListAddr[0].sin_port,numPeer);	//very last peer to communicate gets token
                else
                        sprintf(buffer,"%s:%u:0:%d",inet_ntoa(clntListAddr[n+1].sin_addr),clntListAddr[n+1].sin_port,numPeer);	//peers do not get token
                sendto(s,buffer,sizeof(buffer),0,(struct sockaddr*)&clntListAddr[n],sizeof(clntListAddr[n]));			//message sent to each connecting peer 
        }
}//once messages are sent out, server disconnects and token ring operation is taken up by peers in ring

