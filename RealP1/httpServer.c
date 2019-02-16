//HTTPServer-TCP Connection
//Programmers:	Ali Al-Senan
//		Benjamin Linam
//Code developed on March 5, 2018
//FOR: COP4635 Systems & Networks 2
//
//The file httpServer.c contains code for server program which allows TCP connection to 
//httpClient programs.  The code establishes connection at desired port number which is
//requested as a command line parameter at program execution.  Once a client program has 
//established connection with the server, communication begins.  The server waits until the 
//client passes a request for a specific file via http GET request.  The server code
//determines if it has received an acceptable request by tokenizing and analysing the GET 
//request.  The server code then locates the requested file in the local folder and passes 
//the text as well as an HTTP header back to the requesting client.  Browser-based clients 
//will receive the text as html while clients requesting from ssh will receive a string of 
//text containing HTTP header and entire html file.

#include "serverFunctions.h" //Header file containing all other required header files and functions.

int main(int argc, char **argv)//argv[1] = IP Address, argv[2] = Port Number
{
	printf("\nRun in the following format: ./httpServer (IP_ADDR) (PORT)");
	int tcp_client_socket = 0;
	int tcp_server_socket;   
	
	printf("\n\tCreating Server on %s:%d",argv[1],atoi(argv[2]));
        
	tcp_server_socket = socket(AF_INET, SOCK_STREAM, 0); // Create Server Socket

	struct sockaddr_in tcp_server_address; // Contains Server Address
	struct sockaddr_in tcp_client_address; // Contains Client Address
	
	tcp_server_address.sin_family = AF_INET; // Uses IPv4
	tcp_server_address.sin_port = htons(atoi(argv[2])); // Defines Port Number
	tcp_server_address.sin_addr.s_addr = inet_addr(argv[1]); // Defines IP Address
	
	if(bind(tcp_server_socket,(struct sockaddr *)&tcp_server_address,sizeof(tcp_server_address)) == -1) error(1); // Binds IP:PORT to socket

	if(listen(tcp_server_socket, 32) == -1) error(2); // Sets server listen queue
	communication(tcp_server_socket,tcp_client_socket,(struct sockaddr *)&tcp_client_address,(struct sockaddr *)&tcp_server_address); // Main Server Communication
	
        close(tcp_client_socket);
	close(tcp_server_socket);
    
	return 0;
}
