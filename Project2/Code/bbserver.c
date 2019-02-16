/*Token Ring bbserver.c file
 *Programmers:  Ali Al-Senan
 *              Benjamin Linam
 *Code developed on April 15, 2018
 *FOR: COP4635 Systems & Networks 2
 */


#include"serverFunctions.h"

int main(int argc, char* argv[]){

	if(argc!=3)				//3 arguments are expected for server: ./bbserver <portnum><peernum>
	{
		printf("Error: Three arguments are needed!");
		return 1;
	}
								//build local (server) socket address
	memset(&servAddr,0,sizeof(servAddr));			//allocate memory
	servAddr.sin_family = AF_INET;				//family field
	servAddr.sin_port = atoi(argv[1]);			//default port number
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);		//default IP address
	
	//Create Socket
	if((s = socket(PF_INET,SOCK_DGRAM,0))<0)
	{
		perror("Error: socket failed!");
		return 1;
	}
	//Bind socket to local address and port
	if((bind(s,(struct sockaddr*)&servAddr, sizeof(servAddr))<0))
	{
		perror("Error: bind failed!");
		return 1;
	}
	
	receiveStrings_outgoingMail(atoi(argv[2])); 	//loop will receive expected peer messages and will send each peer the information of their expected neighbor
	printf("\n%d Peers Connected, Exiting!\n",atoi(argv[2]));//display that server is exiting
	return 0;//operation of token ring will be under control of peers after server exits
}
