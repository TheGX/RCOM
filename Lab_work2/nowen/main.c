
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <strings.h>
#include "functions.h"
#include "FTP_functions.h"

int main(int argc, char** argv){
	char	buf[1024];
	int size = 1024;
	char str[size];

	int  socket_cmd  = FTP_connect(SERVER_NAME,SERVER_PORT);
	int* socket_data = 0;

	if(socket_cmd ==0) exit(0);
	
	FTP_read(socket_cmd,str,size);
	FTP_write(socket_cmd,buf,"USER","extrack_root");
	
	FTP_read(socket_cmd,str,size);
	FTP_write(socket_cmd,buf,"PASS","LprosoS");
	FTP_read(socket_cmd,str,size);

	FTP_write(socket_cmd,buf,"EPSV","");
	FTP_read(socket_cmd,str,size);
	printf("V - %d\n",socket_data);
	socket_data = FTP_Mode_PASV(&socket_data,str);
	printf("V - %d\n",socket_data);
	FTP_write(socket_cmd,buf,"CWD","www");
	FTP_read(socket_cmd,str,size);
	printf("V - %d\n",socket_data);
	FTP_write(socket_cmd,buf,"RETR","index.html");
	FTP_read(socket_cmd,str,size);
	printf("V - %d\n",socket_data);

	FTP_Download(socket_data, "index.html");
	
	close(socket_cmd);
	exit(0);
}


