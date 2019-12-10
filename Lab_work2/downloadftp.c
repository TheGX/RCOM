#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#define FTP_PORT 21
#define MAX_STRING 100

// FTP WORKS ON APPLICATION LAYER! MAIN GOAL: MOVING FILES ( in our project we are supposed to only download a file)
//we need to have two sockets: one for control and one for data

/* main steps:

- parse this: clienteDownload ftp://[user]:[pass]@<host_name>/<path_para_o_ficheiro> which is received via arg
- start connection using control socket
- check permissions of the user
- passive mode
- start connection using data socket
- retrieve - send a request for a specific file
- get the file from the server
- quit connection
- */

int connection(char * IP_host, int port);

void parseArg(char * argv, char* user, char *pass, char *hostName, char* pathFile);

void parseFile(char* pathFile, char* fileName);

char* getIp(char* server_name);

int checkConnection(int socket);

int sendCommandUser(int socket, char* user);

int sendCommandPassword(int socket, char* pass);

void readResponse(int socket, char *response);

int sendCommandPasvAndGetPort(int socket);

int sendRetrAndGetFile(int socket, char* filename, char *path, int socket_data);




int main(int argc, char *argv[]){
    if(argc < 2){

        printf("Usage: ftp://[user]:[pass]@<host_name>/<path_to_the_file>");
        exit(1);
    }

    char user[MAX_STRING];
    char pass[MAX_STRING];
    char hostName[MAX_STRING];
    char pathFile[MAX_STRING];
    char pathFileDestroyd[MAX_STRING];
    char fileName[MAX_STRING];

    char *IP_host;
    int sockFd_control,sockFd_data, data_port=0;
    char response[3];
    int aux=0, aux1=0, aux2=0, aux3=0;

    memset(user, 0, MAX_STRING);
    memset(pass, 0, MAX_STRING);
    memset(hostName, 0, MAX_STRING);
    memset(pathFile, 0, MAX_STRING);
    memset(fileName, 0, MAX_STRING);
    
    parseArg(argv[1], user, pass, hostName, pathFile);

    strcpy(pathFileDestroyd,pathFile);
    parseFile(pathFileDestroyd, fileName);

    printf("-> user:%s\n", user);
    printf("-> pass:%s\n", pass);
    printf("-> host name:%s\n", hostName);
    printf("-> pathFile:%s\n", pathFile);
    printf("-> fileName:%s\n\n\n", fileName);

    IP_host=getIp(hostName);    

    /*if(IP_host == NULL){

        perror("getIP():");
    }*/
    printf("> Trying: %s...\n", IP_host);

    sockFd_control=connection(IP_host, FTP_PORT);

    checkConnection(sockFd_control);

    aux1=sendCommandUser(sockFd_control, user);


    if(aux1==1){

         aux2=sendCommandPassword(sockFd_control, pass);
    }
    else{

        close(sockFd_control);
        exit(1);
        return 0;
    }

    if(aux2==1){

        data_port=sendCommandPasvAndGetPort(sockFd_control);
    }
    else {

        close(sockFd_control);
        exit(1);
    }

    
    /*********** PASSIVE FROM NOW ON************/

    sockFd_data=connection(IP_host, data_port);
    //aux3= checkConnection()    -> preciso de checkar?

    aux3=sendRetrAndGetFile(sockFd_control, fileName, pathFile, sockFd_data);

    if(aux3==1){

        write(sockFd_control,"quit",4);
        printf("> Goodbye camarada!\n");
    }

    else{
        close(sockFd_control);
        close(sockFd_data);
        exit(1);
    }

    close(sockFd_control);
    close(sockFd_data);
    return 0;

}


char* getIp(char* server_name){
    struct hostent *h;

    if ((h = gethostbyname(server_name)) == NULL){
			herror("gethostbyname");
			exit(1);
    }

    //printf("V - Host name  : %s\n", h->h_name);
    //printf("V - IP Address : %s\n", inet_ntoa(*((struct in_addr *)h->h_addr)));

    return inet_ntoa(*((struct in_addr *)h->h_addr));
}

int connection(char * IP_host, int port){

    int	sockfd;
	struct	sockaddr_in server_addr;

    /*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET; //IPV4
    server_addr.sin_addr.s_addr = inet_addr(IP_host);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(port);		/*server TCP port must be network byte ordered */

    /*open an TCP socket*/
	if ((sockfd= socket(AF_INET,SOCK_STREAM,0)) < 0) {
    		perror("socket()");
        	exit(0);
    }

    /*connect to the server*/
    if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        	perror("connect()");
		    exit(0);
	}

    return sockfd;

}

void parseArg(char * argv, char * user, char* pass, char* hostName, char* fileName){

    int l=0;
    char* real= strstr(argv,"ftp://");
    real = real + 6;

    char *user_aux=strtok(real, ":");
    for(int i=0; i<strlen(user_aux); i++){
        user[l++]=*(user_aux+i);
    }

    char *pass_aux= strtok(0,"@");
    l=0;
    for(int i=0; i<strlen(pass_aux); i++){    
        pass[l++]=*(pass_aux+i);
    }

    l=0;
    char *host_aux= strtok(0, "/");
    for(int i=0; i<strlen(host_aux); i++){
        hostName[l++]=*(host_aux+i);
    }

    l=0;
    char *fileName_aux =strtok(0,"");
    for(int i=0; i<strlen(fileName_aux); i++){
        fileName[l++]=*(fileName_aux+i);
    }
}

void parseFile(char* pathFile, char* fileName){

   
    char *user_aux;
    char *fileName_aux;
    int l=0;

    user_aux=strtok(pathFile, "/");

    while((user_aux=strtok(0, "/"))!= NULL){
       
        fileName_aux=user_aux;
    }

    for(int i=0; i<strlen(fileName_aux); i++){    
        fileName[l++]=*(fileName_aux+i);
    }
    
}

void readResponse(int socketfd, char *responseCode){

    int state = 0;
	int i = 0;
	char c;
    int done=0;

	while (!done)
	{	
		read(socketfd, &c, 1);
		//printf("%c", c);
		switch (state)
		{
		//waits for 3 digit number followed by ' ' or '-'
		case 0:
			if (c == ' ')
			{
				if (i != 3)
				{
					printf("> Error receiving response code\n");
					return;
				}
				i = 0;
				state = 1;
			}
			else
			{
				if (c == '-')
				{
					state = 2;
					i=0;
				}
				else
				{
					if (isdigit(c))
					{
						responseCode[i] = c;
						i++;
					}
				}
			}
			break;
		//reads until the end of the line
		case 1:
			if (c == '\n')
			{
				done=1;
			}
			break;
		//waits for response code in multiple line responses
		case 2:
			if (c == responseCode[i])
			{
				i++;
			}
			else
			{
				if (i == 3 && c == ' ')
				{
					state = 1;
				}
				else 
				{
				  if(i==3 && c=='-'){
					i=0;
					
				}
				}
				
			}
			break;
		}
	}
}

int checkConnection(int socket){

    char response[3];
    memset(response,0,4);

    readResponse(socket, response);


    if(strcmp(response,"220")==0){

        printf("> Connected to the server!\n> Sending User...\n");
        return 1;

    }

    else{

        printf("> Failed to connect to server\n > Exiting...\n");
        return 0;
    }

}

int sendCommandUser(int socket, char* user){

    char response[3];
    memset(response,0,4);


    write(socket,"user ",5);
    write(socket,user, strlen(user));
    write(socket,"\r\n",2);

    readResponse(socket, response);
    
    if( strcmp(response, "331")==0){

        printf("> Sending password...\n");
        return 1;
    }

    else{

        printf("> Failed to send user\n> Exiting...\n ");
        return 0;

    } 
    
}

int sendCommandPassword(int socket, char* pass){

    char response[3];
    memset(response,0,4);


    write(socket,"pass ",5);
    write(socket,pass, strlen(pass));
    write(socket,"\r\n",2);

    readResponse(socket, response);
    
    if( strcmp(response, "230")==0){

        printf("> Logged in\n");
        return 1;
    }

    else{

        printf("> Failed to send pass\n> Exiting...\n ");
        return 0;

    } 

}


int sendCommandPasvAndGetPort(int socket){

    char response[100];// since we know what we are supposed to receive in this state
    memset(response,0,100);
    char *responseCode;
    char *resultStr;
    char * resultStrtok;
    char c;
    int l=0, j=0;
    int getDigits[6];

    write(socket, "pasv\r\n", 6);

    while(c != ')'){

        read(socket, &c, 1);  
        response[l++]=c;
    }

    responseCode=strtok(response, " ");


    if(strcmp(responseCode, "227")==0){

        printf("> Pasv mode okay. Calculating port...\n");
        resultStr=strtok(0, "");

        resultStr=resultStr+1;
        resultStrtok=strtok(resultStr, ",");
        getDigits[j]=atoi(resultStrtok);

        j++;

        while( j < 6){
            resultStrtok=strtok(0, ",");
            getDigits[j]=atoi(resultStrtok);
            j++;   
        }
    }

    printf("> Port: %d\n", getDigits[4]*256+getDigits[5]);
    return getDigits[4]*256+getDigits[5];
}

int sendRetrAndGetFile(int socket, char* filename, char *path, int socket_data){

    char response[3];
    memset(response,0,4);

    FILE *file= fopen((char*)filename, "wb+");
    int bytes;
    char buff[1000];

    write(socket,"retr ",5);
    write(socket,path, strlen(path));
    write(socket,"\r\n",2);

    readResponse(socket, response);

    if(strcmp(response, "150")==0){

        printf("> File OKAY. Downloading...\n");
        
        while((bytes= read(socket_data, buff,100))>0){
            bytes=fwrite(buff,bytes,1,file);
            if(bytes<0){

                printf("Error writing to the file\n");
            }
        }

        printf("> File downloading complete!\n");
        fclose(file);   
        return 1;

    }
    else if(strcmp(response, "550")==0){

        printf("> No such file!\n> Exiting...\n");
        fclose(file);   
        return 0;
    }
    else {

        fclose(file);
        return 0;
    }

}
