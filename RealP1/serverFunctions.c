//serverFunctions.c-TCP Connection
//Programmers:	Ali Al-Senan
//		Benjamin Linam
//Code developed on March 5, 2018
//FOR: COP4635 Systems & Networks 2

#include "serverFunctions.h"

// communication
// param 1: Server Socket (int)
// param 2: Client Socket (int)
// param 3: Client Address (struct sockaddr*)
// param 4: Server Address (struct sockaddr*)
void communication(int tcp_server_socket,int tcp_client_socket,struct sockaddr* tcp_client_address,struct sockaddr* tcp_server_address)
{
	char tcp_server_response[MAXSIZE];
	char tcp_client_message[MAXSIZE];
	
        char temp[BUFFER];
        char get[4*CONTENT_LENGTH], path[CONTENT_LENGTH], http[CONTENT_LENGTH], extension[CONTENT_LENGTH], contentLength[CONTENT_LENGTH], contentType[CONTENT_LENGTH];
        char *orgGet, *orgPath, *orgHttp, *orgExtension;
	char *t = malloc(50);
	int n,ext,m,file;
        long len = 0;
        long block = 0;
	char client[6] = {'C','l','i','e','n','t'};
	int isClient = 0;
	int iteration = 0;
        socklen_t sockLen;
  
	tcp_client_socket = accept(tcp_server_socket,(struct sockaddr *)&tcp_client_address,&sockLen); // Accepts communication assigns socket
	while(1)
	{
		iteration++;
                ext = 0;
                sockLen = sizeof(tcp_client_address);
                
                if(0)
		{
			BACK:
			tcp_client_socket = accept(tcp_server_socket,(struct sockaddr *)&tcp_client_address,&sockLen);
		}
                
                if(isClient == 0 && iteration != 1) // Browser Setup
		{
			close(tcp_client_socket);
			tcp_client_socket = accept(tcp_server_socket,(struct sockaddr *)&tcp_client_address,&sockLen);
		}
			
		if((recvfrom(tcp_client_socket,&tcp_client_message,MAXSIZE,0,(struct sockaddr *)&tcp_client_address,&sockLen))<=0)goto BACK; // Recieve message from client, goto BACK if no message is recieved
                
		isClient = 0;
		for(n=0;n<strlen(tcp_client_message);++n) // Checks if string "Client" is in the tcp_client_message
		{
			for(m=0;m<6;++m)
			{
				if(client[m] == tcp_client_message[n+m])
				{
					if(m == 5)
						isClient = 1;
				}
				else break;
			}
			if(isClient)break;
		}
		
		
		printf("\n\n-----------------------------------------------------\n");
		
		// Tokenize tcp_client_message
		orgGet = strtok(tcp_client_message," \n");
                strcpy(get,orgGet);
		if(!strcmp(get,"ding:"))continue;
                
		orgPath = strtok(NULL," \n");
                strcpy(path,orgPath);
		orgHttp = strtok(NULL," \n");
                strcpy(http,orgHttp);

		printf("\nCurrent Request: %s %s %s",tcp_client_message,path,http);
		orgPath++;
                if(!strcmp(path,"/")) strcat(orgPath,"index.html"); // "/" equivalent to "/index.html"
                
                strncpy(tcp_server_response, "HTTP/1.1 200 OK\n", MAXSIZE); // Add to tcp_server_response

		strcpy(t,orgPath); // Check if there is an extension
                for(n=0;n<CONTENT_LENGTH;++n)
                        if(path[n] == '.')
                                ext = 1;
                
                strcpy(contentType,"Content-Type: text/html"); 
                if(ext) // Determine extension type and assign the correct Content-Type header
                {
                        orgExtension = strtok(path,".");
                        strcpy(extension,orgExtension);
                        orgExtension = strtok(NULL,".\n");
                        strcpy(extension,orgExtension);
                        
                        if(!strcmp(extension,"html"))
                                strcpy(contentType,"Content-Type: text/html");
                        else if(!strcmp(extension,"htm"))
                                strcpy(contentType,"Content-Type: text/html");
                        else if(!strcmp(extension,"jpg"))
                                strcpy(contentType,"Content-Type: image/jpg");
                        else if(!strcmp(extension,"jpeg"))
                                strcpy(contentType,"Content-Type: image/jpeg");
                        else if(!strcmp(extension,"png"))
                                strcpy(contentType,"Content-Type: image/png");
                        else if(!strcmp(extension,"c"))
                                strcpy(contentType,"Content-Type: text/x-c");
                        else if(!strcmp(extension,"ico"))
                        {
                                strncpy(tcp_server_response, "HTTP/1.1 204 No Content\nConnection: close\r\n\r\n", MAXSIZE);
                                printf("\nSending Header:\n%s",tcp_server_response);
                                send(tcp_client_socket,tcp_server_response,sizeof(tcp_server_response),0);
                                printf("\nSent Data\n");
                                continue;
                        }
                }
		
		if((file = open(orgPath,O_RDONLY))==-1) // Opens the file and checks if valid
                {
                        strncpy(tcp_server_response, "HTTP/1.1 404 Not Found\nConnection: close\r\n\r\n404 Not Found", MAXSIZE);
                        printf("\nSending Header:\n%s",tcp_server_response);
                        send(tcp_client_socket,tcp_server_response,sizeof(tcp_server_response),0);
                        printf("\nSent Data\n");
                        continue;
                }

		printf("\n\n\tFile %s Opened!\n\n",orgPath);
                
		len = (long)lseek(file,(off_t)0,SEEK_END); // Determine file size
		lseek(file,(off_t)0,SEEK_SET);
                sprintf(contentLength,"Content-Length: %ld", len);
                sprintf(tcp_server_response,"HTTP/1.1 200 OK\nConnection: keep-alive\n%s\n%s\r\n\r\n",contentType,contentLength); // Assign correct headers for the response
                printf("\nSending Header:\n%s",tcp_server_response);
                write(tcp_client_socket,tcp_server_response,strlen(tcp_server_response)); // Sends the headers to the client
                
                memset(temp,0,BUFFER); // clear first BUFFER temp characters
                while((block = read(file,temp,BUFFER))>0)//Send file in blocks
                {
                        write(tcp_client_socket,temp,block); // Sends the file in chunks of 12KB
                        memset(temp,0,BUFFER); // clear first BUFFER temp characters
                }
                temp[len%BUFFER] = '\0';

		printf("Sent Data!\n");
                close(file);
                
                for(n=0;n<MAXSIZE;++n)tcp_client_message[n] = 0;
	}
	free(t);
}

// error
// param 1: Error Type (int)
void error(int e)
{
	switch(e)
	{
		case 1:
		printf("\n\nError: Binding Error!\n\n");
		exit(1);
		case 2:
		printf("\n\nError: Listening Error!\n\n");
		exit(1);
	}
}
