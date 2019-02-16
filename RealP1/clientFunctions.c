//clientFunctions.c-TCP Connection
//Programmers:	Ali Al-Senan
//		Benjamin Linam
//Code developed on March 5, 2018
//FOR: COP4635 Systems & Networks 2

#include "clientFunctions.h"
char *template = "GET /(yourfile.html) HTTP/(version#)\n\n";//template string to display for user 
char *currString = " ";
char HTTPCPY[ARRLEN] = " ";
int PORTNUM = 0;

void serverCommunication(int tcp_client_socket){
	while(1){							//menu that program will loop to after error or successful communication
		int userChoice = 1;
		char*HTTPrequest = malloc(sizeof(char)*ARRLEN);
		char tcp_server_response[LONGFILE];	
		printf("\n\nWelcome to HTTP request program!\n");
		printf("\t\tTo request file from server - (1)\n\t\tTo terminate program - (0)\n\t\tENTER: ");
		scanf("%d", &userChoice);
		if(userChoice == 1){
			printf("Please enter request using the following format (excluding parenthes): %s",template);
			scanf("\n%[^\n]s",HTTPrequest);
			strcpy(HTTPCPY,HTTPrequest);			//copies input string for editing during strtok operations
			currString = strtok(HTTPCPY," \n");
			if(strcmp(currString,"GET")!= 0){		//determines if GET request is acceptable, if not, branch to menu
				printf("INVALID REQUEST\n");
				continue;
			}
			currString = strtok(NULL," \n");		//----->
			if(currString[0] !='/'){
				printf("INVALID PATH\n");
				continue;
			}
			currString = strtok(NULL," \n");		//----->
			if(strcmp(currString,"HTTP/1.1") != 0){
				printf("INVALID PROTOCOL\n");
				continue;
			}
			printf("Request received: %s\n",HTTPrequest);
			strcat(HTTPrequest,"\nUser-Agent: Client\r\n\r\n");	//adds client-type header for server to recognize request
    				
			send(tcp_client_socket, HTTPrequest, strlen(HTTPrequest),0);	//sends request to server
			recv(tcp_client_socket, &tcp_server_response, sizeof(tcp_server_response), 0);   //receives response from server
    					
    			printf("\n\n Server says: %s \n", tcp_server_response); 	//outputs response from server
			
			free(HTTPrequest);//free memory allocated for request string 
		}
		else if(userChoice == 0){
			close(tcp_client_socket);//close socket and terminate program
			free(HTTPrequest);
			return;
		}
		else{//	default:
			printf("Input not recognized\n\n");
			userChoice = 1;
		}
	}
}
