
//HTTPClient-TCP Connection
//PROGRAMMERS: 	Ali Al-Senan
//		Benjamin Linam
//Code developed on March 5, 2018
//FOR: COP4635 Systems & Networks 2
//
//The file httpClient.c contains code for client program which allows TCP connection to
//httpServer program. The code prompts user for desired port number and displays a template 
//for standard http GET request and allows user to manually input desired .html file name 
//and http protocol version. Connection to server at desired port and IP is established.
//Errors are tested and if connection and communication are successful, the desired file is 
//returned to client and displayed for user. A menu is provided for user to choose to 
//continue to request files from server or close connection.  
#include "clientFunctions.h"

int main(int argc, char **argv){
	printf("\nRun in the following format: ./httpClient (IP_ADDR) (PORT)");
							//creating the TCP socket
    	int tcp_client_socket;                          //Socket descriptor
    	tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0);      //Calling the socket function - args: socket domain, socket stream type, TCP protocol (default
    									//specify address and port of the remote socket
   	struct sockaddr_in tcp_server_address;             //declaring a structure for the address
    	tcp_server_address.sin_family = AF_INET;           //Structure Fields' definition: Sets the address family of the address the client would connect to
    	tcp_server_address.sin_port = htons(atoi(argv[2]));        //Specify and pass the port number to connect - converting in right network byte order
    	tcp_server_address.sin_addr.s_addr = inet_addr(argv[1]);   //Connects to requested IP
 	int connection_status = connect(tcp_client_socket, (struct sockaddr *) &tcp_server_address, sizeof(tcp_server_address));   //connects to waiting server 
	if(connection_status == -1){  //return value of 0 means all okay, -1 means a problem
        	printf(" Problem connecting to the socket! Sorry!! \n");
		return 0;
     	}
	else{
		serverCommunication(tcp_client_socket);//function that handles communication with server
		return 0;
	}
}
	



