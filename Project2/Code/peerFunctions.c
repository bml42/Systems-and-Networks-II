/*Token Ring peerFunctions.c file
*Programmers:  Ali Al-Senan
*              Benjamin Linam
*Code developed on April 15, 2018
*FOR: COP4635 Systems & Networks 2
*/

#include "peerFunctions.h"

int numOfHosts = 0;
int token = 0;
int s;
int optionEdited = 0;
int start = 1;
char option = 0;
const char delimiter[3] = ":\0";
pthread_mutex_t lock;
struct sockaddr_in neighborAddr;

//inputThread
//
//Parameter 1: char*, Name of the file being used by the peer.
//Return: None
void *inputThread(void *a)
{
	char* fileName = (char*)a;
        FILE *fp;
        int read;
        char stringWrite[messagelen] = {0};
        char stringWriteCopy[messagelen] = {0};
        char stringRead[messagelen] = {0};
        char stringReadCopy[messagelen] = {0};
        char* stringNumber;
        char* tempParse;
        char temp;
	int n=0;

        while(1)
        {
                printf("\nw - Write - Appends a new message to the end of the message board.\nr - Read - Reads a particular message from the message board using a message sequence number.\nl - List - Displays the range of a valid sequence numbers of messages posted to the board.\ne - Exit - Closes the message board.\n\n");
                scanf("\n%c",&option);
fflush(stdin);
                pthread_mutex_lock(&lock);
                optionEdited = 1;
                pthread_mutex_unlock(&lock);

                switch(option)
                {
                        case 'W':
                        case 'w'://Writes to the bulletin board.
                                printf("\nEnter message into the following format:\n<message n=#>...</message>\n");
                                scanf("\n%[^\n]s",stringWrite);		//Takes in full line.

                                while(!token){}				//Waits for token.
                                fp = fopen(fileName,"r");
                                fseek(fp,0,SEEK_SET);
                                for(n=1;;++n)				//Saves last line of file
                                {
                                        temp = getc(fp);
                                        if(temp == -1)
                                                break;
                                        temp = getc(fp);
                                        if(temp == -1)
                                                break;
                                        fseek(fp,-2,SEEK_CUR);
                                        fscanf(fp,"\n%[^\n]s",stringRead);
                                }

				//Checks formatting.
                                strcpy(stringWriteCopy,stringWrite);
                                tempParse = strtok(stringWriteCopy,"=");
                                if(strcmp(tempParse,"<message n"))
                                {
                                        printf("\nInvalid Format!\n");
                                        break;
                                }
                                tempParse = strtok(NULL,">");
                                tempParse[strlen(tempParse)] = '>';
                                if(tempParse[1] != '>' && tempParse[2] != '>' && tempParse[3] != '>' && tempParse[4] != '>')//Can go up to 9999 sequences
                                {
                                        printf("\nInvalid Format!\n");
                                        break;
                                }
                                if(atoi(tempParse)==0 && tempParse[0]!='0')
                                {
                                        printf("\nInvalid Format!\n");
                                        break;
                                }
                                else if(atoi(tempParse)!=n)//Checks sequence number.
                                {
                                        printf("\nIncorrect Sequence Number!\nNext Sequence Number: %d\n",n);
                                        break;
                                }
                                tempParse = strtok(NULL,"<");
                                tempParse = strtok(NULL,">");
                                if(strcmp(tempParse,"/message"))
                                {
                                        printf("\nInvalid Format!\n");
                                        break;
                                }

                                fp = fopen(fileName,"a");//Appends to file.
                                fprintf(fp,"%s\n",stringWrite);
                        	fseek(fp,0,SEEK_SET);
				fscanf(fp,"%[^EOF]s",stringWrite);
				fclose(fp);
                                break;
                        case 'R':
                        case 'r'://Reads specific message from file using sequence number.
                                printf("\nEnter message sequence number: ");
                                scanf("\n%d",&read);
                                while(!token){}	//Waits for token.
                                fp = fopen(fileName,"r");
                                fseek(fp,0,SEEK_SET);
                                while(1)
                                {
                                        temp = getc(fp);
                                        if(temp == -1)
                                        {
                                                printf("\nInvalid Sequence Number!\n");
                                                break;
                                        }
                                        temp = getc(fp);
                                        if(temp == -1)
                                        {
                                                printf("\nInvalid Sequence Number!\n");
                                                break;
                                        }
                                        fseek(fp,-2,SEEK_CUR);
                                        fscanf(fp,"\n%[^\n]s",stringRead);//Reads full line.
                                        strcpy(stringReadCopy,stringRead);
                                        strtok(stringRead,"=>");
                                        stringNumber = strtok(NULL,"=>");
                                        if(atoi(stringNumber) == read)//Checks if sequence number matches.
                                        {
                                                printf("\n%s\n",stringReadCopy);
                                                break;
                                        }
                                }
                                fclose(fp);
                                break;
                        case 'L':
                        case 'l'://Lists the valid sequence numbers.
                                while(!token){} //Waits for token.
                                fp = fopen(fileName,"r");
                                fseek(fp,0,SEEK_SET);
                                n = 0;
				temp = 0;
                                while(1)
                                {
                                        temp = getc(fp);
                                        if(temp == -1)
                                        {
                                                printf("\nSequence Numbers: None\n");
                                                break;
                                        }
                                        temp = getc(fp);
                                        if(temp == -1)
                                        {
                                                printf("%d\n",atoi(stringNumber));
                                                break;
                                        }
                                        fseek(fp,-2,SEEK_CUR);
                                        fscanf(fp,"\n%[^\n]s",stringRead);//Reads full line.
                                        strtok(stringRead,"=>");
                                        stringNumber = strtok(NULL,"=>");
                                        if(!n)
                                                printf("\nSequence Numbers: %d to ",atoi(stringNumber));
                                        n++;
                                }
                                fclose(fp);
                                break;
                        case 'E':
                        case 'e': //Exit Program.
                                while(!token){}//Waits for token.
                                pthread_mutex_lock(&lock);
                                optionEdited = 0;
                                pthread_mutex_unlock(&lock);
                                pthread_exit(NULL);//Exits Thread.
                                break;
                        default:
                                printf("\nInvalid Input!\n");
                                break;
                }
                pthread_mutex_lock(&lock);
                optionEdited = 0;
                pthread_mutex_unlock(&lock);
        }
}

//setConnection
//
//Parameter 1: struct hostent, used to aquire the IP of the connection.
//Parameter 2: int, used to send in the port number of the connection.
//Return: struct sockaddr_in, the neighbors address structure.
struct sockaddr_in setConnection(struct hostent *name,int port)
{
	struct sockaddr_in p;
	p.sin_family = AF_INET;
	inet_pton(AF_INET,(*name).h_addr_list[0],&p.sin_addr);
	p.sin_port = port;
	return p;
}

//firstContact
//
//Parameter 1: struct sockaddr_in*, sends in the host information.
//Parameter 2: struct hostent, used to aquire the IP of the connection.
//Parameter 3: int, used to send in the port number of the connection.
//Parameter 4: char*, send in main argument 5 to check whether it's joining an existing ring.
//Return: struct sockaddr_in, the neighbors address structure.
struct sockaddr_in firstContact(struct sockaddr_in *host,struct hostent *name,int port,char *arg)
{
	struct sockaddr_in servAddr,neighborAddr;
	char neighborString[messagelen] = {0};
	char *neighborIP,*neighborPort;
        memset(neighborString,0,sizeof(neighborString));
	
	if(arg != 0)//there is a sixth parameter
        {
                if(!strcmp(arg,"-new"))//Connecting to an already established ring
                {
                        servAddr = setConnection(name,port);
                        sprintf(neighborString,"&%s:%d",inet_ntoa(host->sin_addr),host->sin_port);
                	sendto(s,neighborString,strlen(neighborString),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
		}
                else//Connecting to server for an unestablished ring
                {
                        servAddr = setConnection(name,port);
                        sprintf(neighborString,"Connecting");
			sendto(s,neighborString,strlen(neighborString),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
                }
        }
        else//five parameters, connecting to server for an unestablished ring
        {
                servAddr = setConnection(name,port);
                sprintf(neighborString,"Connecting");
		sendto(s,neighborString,strlen(neighborString),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
        }

	memset(neighborString,0,sizeof(neighborString));//clear string for receiving info from server/peer
        recvfrom(s,neighborString,sizeof(neighborString),0,NULL,NULL);//string containing information for neighbor peer
	
        neighborIP = strtok(neighborString,delimiter);//string is broken down to obtain IP, port, Y/N token, and number of peers
        neighborPort = strtok(NULL,delimiter);
        token = atoi(strtok(NULL,delimiter));
        numOfHosts = atoi(strtok(NULL,delimiter));
        printf("\n\tNumber of Hosts: %d\n",numOfHosts);

        memset(&neighborAddr,0,sizeof(neighborAddr));//setting struct for each peer's next neighbor
        neighborAddr.sin_family = AF_INET;
        inet_pton(AF_INET,neighborIP,&neighborAddr.sin_addr);
        neighborAddr.sin_port = atoi(neighborPort);

	return neighborAddr;//returns a peer's neighbor
}

//loopingCommunication
//
//Parameter 1: char*, send in main argument 5 to check whether it's joining an existing ring.
//Parameter 2: char*, Name of the file being used by the peer.
//Return: None
void loopingCommunication(char *arg,char* fileName)
{
	char *neighborIP,*neighborPort;
        char neighborString[messagelen] = {0},fileString[messagelen] = {0},statusString[messagelen] = {0},temp2String[messagelen] = {0};
	int n,m,len,countdown = 0;
	const char delimiter[3] = ":\0";
	FILE *fp;


        while(1)
        {
                if(token)//peer has access to token 
                {
                        while(optionEdited){}

                        memset(fileString,0,sizeof(fileString));
                        memset(statusString,0,sizeof(statusString));

                        if(start == 1 && arg != 0)
                        {
                                if(!strcmp(arg,"-new"))//connecting to established ring 
                                {
                                        sprintf(statusString,"|0:0:%d",(numOfHosts-2)*(-1));
                                        token = 0;
					sendto(s,statusString,strlen(statusString),0,(struct sockaddr*)&neighborAddr,sizeof(neighborAddr));
                                        start = 0;
                                        continue;
                                }
                        }

                        if(access(fileName,F_OK) == -1)//File Doesn't Exist
                                sendto(s,"|0:0:0",strlen("|0:0:0"),0,(struct sockaddr*)&neighborAddr,sizeof(neighborAddr));
                        else
                        {
                                fp = fopen(fileName,"r");
                                fseek(fp,0,SEEK_SET);
                                fscanf(fp,"%[^EOF]s",fileString);
                                fclose(fp);
                        }

                        if(option == 'e' || option == 'E')//exitting from ring
                                sprintf(statusString,"|%s:%u:%d",inet_ntoa(neighborAddr.sin_addr),neighborAddr.sin_port,numOfHosts-1);
                        else if(countdown != 0)
                        {
                                sprintf(statusString,"|%s:%s:%d",neighborIP,neighborPort,countdown);
                                countdown = 0;
                        }
                        else
                                sprintf(statusString,"|0:0:0");
                        strcat(fileString,statusString);
                        token = 0;
			sendto(s,fileString,strlen(fileString),0,(struct sockaddr*)&neighborAddr,sizeof(neighborAddr));
                        if(option == 'e' || option == 'E')
                                return;
                }
                else//not currently accessing token, can check for new connections
                {
                        memset(fileString,0,sizeof(fileString));
                        memset(statusString,0,sizeof(statusString));
                        memset(neighborString,0,sizeof(neighborString));
                        while(1)
                        {
                                memset(fileString,0,sizeof(fileString));
                                recvfrom(s,fileString,sizeof(fileString),0,NULL,NULL);
				if(!strcmp(fileString,"Connecting")) continue;
                                if(fileString[0] == '&')//&IP:PORT
                                {//new peer attempting to connect, this establishes its place in the ring and sends information for redirecting ring
                                        sprintf(neighborString,"%s:%d:0:%d",inet_ntoa(neighborAddr.sin_addr),neighborAddr.sin_port,++numOfHosts);
                                        neighborIP = strtok(fileString+1,delimiter);
                                        neighborPort = strtok(NULL,delimiter);
                                        printf("\n\tNumber of Hosts: %d\n",numOfHosts);
                                        memset(&neighborAddr,0,sizeof(neighborAddr));
                                        neighborAddr.sin_family = AF_INET;
                                        inet_pton(AF_INET,neighborIP,&neighborAddr.sin_addr);
                                        neighborAddr.sin_port = atoi(neighborPort);
                                        sendto(s,neighborString,strlen(neighborString),0,(struct sockaddr*)&neighborAddr,sizeof(neighborAddr));
                                }
                                else
                                        break;
                        }

                        if(fileString[0] == '|')//adding to an already established bb
                                fp = fopen(fileName,"a");
                        else
                                fp = fopen(fileName,"w");
                        fseek(fp,0,SEEK_SET);
                        len = strlen(fileString);
                        for(n=0;n<len;++n)
                        {
                                if(fileString[n-1] == '|')
                                {
                                        for(m=n;m<len;++m)
                                                temp2String[m-n] = fileString[m];
                                        temp2String[m-n] = '\0';
                                        break;
                                }
                        }
                        if(fileString[0] != '|')
                                fprintf(fp,"%*.*s",n-1,n-1,fileString);
                        fclose(fp);

                        neighborIP = strtok(temp2String,delimiter);//reassigns next neighbor
                        neighborPort = strtok(NULL,delimiter);
                        countdown = atoi(strtok(NULL,delimiter));

                        if(countdown == 1)//exitting from ring, reassigns new neighbors
                        {
                                memset(&neighborAddr,0,sizeof(neighborAddr));
                                neighborAddr.sin_family = AF_INET;
                                inet_pton(AF_INET,neighborIP,&neighborAddr.sin_addr);
                                neighborAddr.sin_port = atoi(neighborPort);

                                countdown = 0;
                                printf("\n\tNumber of Hosts: %d\n",--numOfHosts);
                        }
                        else if(countdown > 0)//peer has requested to leave, informing each peer
                        {
                                printf("\n\tNumber of Hosts: %d\n",--numOfHosts);
                                countdown--;
                        }
                        else if(countdown == -1)//new peer is connected to ring, hosts increase
                        {
                                printf("\n\tNumber of Hosts: %d\n",++numOfHosts);
                                countdown = 0;
                        }
                        else if(countdown < 0)//informing each peer that a new peer is attempting to join
                        {
                                printf("\n\tNumber of Hosts: %d\n",++numOfHosts);
                                countdown++;
                        }
                        fflush(stdout);

                        token = 1;//token is released for peers
                }
        }
}
