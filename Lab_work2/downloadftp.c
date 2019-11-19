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

#define FTP_PORT 21

// FTP WORKS ON APPLICATION LAYER! MAIN GOAL: MOVING FILES ( in our project we are supposed to only download a file)
//we need to have two sockets: one for control and one for data

/* main steps:

- get user and password (maybe send this via argv?)
- start connection using control socket
- check permissions of the user
- passive mode
- start connection using data socket
- retrieve - send a request for a specific file
- get the file from the server
- quit connection
- */

int connection();


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

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}

int connection(){

    int	sockfd;
	struct	sockaddr_in server_addr;

    /*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET; //IPV4
    //server_addr.sin_addr.s_addr = inet_addr(FTP_PORT);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(FTP_PORT);		/*server TCP port must be network byte ordered */

    /*open an TCP socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    		perror("socket()");
        	exit(0);
    }

    /*connect to the server*/
    if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        	perror("connect()");
		    exit(0);
	}

    printf("Connection openned!\n");
    return sockfd;

}
