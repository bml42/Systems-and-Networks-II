/*Token Ring bbpeer.c file
*Programmers:  Ali Al-Senan
*              Benjamin Linam
*Code developed on April 15, 2018
*FOR: COP4635 Systems & Networks 2
*/


#include "peerFunctions.h"

int main(int argc, char* argv[]) //./bbclient localhost <serverport> <peerport> <filename> <-new>
{
	pthread_mutex_init(&lock,NULL);					//Initializing mutex lock
	pthread_t tidI;
	struct hostent *servName = gethostbyname(argv[1]);		//Setup host IP
	int servPort = atoi(argv[2]),hostPort = atoi(argv[3]);
	struct sockaddr_in hostAddr;
	char *fileName = argv[4];

	if(argc!=6 && argc!=5)//Checks whether the required number of arguments is received.
	{
		printf("Error: Five or Six arguments are needed!");
		return 1;
	}

	memset(&hostAddr,0,sizeof(hostAddr));
	//Sets up hostAddr structure.
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	hostAddr.sin_port = hostPort;	

	if((s = socket(PF_INET,SOCK_DGRAM,0))<0)//Creates Socket.
	{
		perror("Error: Socket failed!");
		return 1;
	}

	if((bind(s,(struct sockaddr*)&hostAddr, sizeof(hostAddr))<0))//Binds socket to local addressing port.
	{
		perror("Error: bind failed!");
		return 1;
	}

	//Sets up the neighbor address by communicating with either a peer or a server.
	neighborAddr = firstContact(&hostAddr,servName,servPort,argv[5]);
	printf("Neighbor IP: %s NeighborPort: %u",inet_ntoa(neighborAddr.sin_addr),neighborAddr.sin_port);
	//Creates a thread that handles the bulletin board.
	pthread_create(&tidI,NULL,inputThread,(void*)fileName);

	//Main thread handling token passing and communication.
	loopingCommunication(argv[5],fileName);

	pthread_mutex_destroy(&lock);
	close(s);
	return 0;
}
