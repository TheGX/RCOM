#include "FTP_functions.h"

char* FTP_read(int socket_cmd,char* str,int size){
	FILE* fp = fdopen(socket_cmd, "r");
	clearBuffer(str,size);
	while (!('1' <= str[0] && str[0] <= '5') || str[3] != ' '){
		memset(str, 0, size);
		*str = *fgets(str, size, fp);
	} 
	printf("%s", str);
	return str;
}

int FTP_write(int socket_cmd,char* buf,char* CMD,char* msg){
	int bytes;
	sprintf(buf,"%s %s%s",CMD,msg,"\r\n");
	bytes = write(socket_cmd, buf, strlen(buf));
	printf("V - Bytes written %d.\nV - %s.\n", bytes);
	return bytes;
}

int FTP_connect(char* server_name, int server_port){
	int socket_cmd;
	/*server address handling*/
	struct	sockaddr_in server_addr;
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(getIp(server_name));	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(server_port);		/*server TCP port must be network byte ordered */
  
		/*open an TCP socket*/
	if ((socket_cmd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
			perror("socket()");
			exit(0);
	}
	
		/*connect to the server*/
	if(connect(socket_cmd,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
			perror("connect()");
			exit(0);
	}
	
	printf("V - Connection open !\n");
	return socket_cmd;
}

int FTP_Mode_PASV(int socket_data,char* str){
	int port;
	if ((sscanf(str, "229 Entering Extended Passive Mode (|||%d|)",&port)) < 0)
		printf("ERROR: Cannot process information to calculating port.\n");

	printf("V - IP:Port = %s:%d\n",getIp(SERVER_NAME),port);
	
	if ((socket_data = FTP_connect(getIp(SERVER_NAME), port)) < 0)
			printf("ERROR: Incorrect file descriptor associated to ftp data socket fd.\n");

	return socket_data;
}

int FTP_Download(int data_socket_fd, const char* filename){
  FILE* file;
	int bytes;

	if (!(file = fopen(filename, "a"))) {
		printf("ERROR: Cannot open file.\n");
		return 1;
	}

	char buf[1024];
	while ((bytes = read(data_socket_fd, buf, sizeof(buf)))) {
		if (bytes < 0) {
			printf("ERROR: Nothing was received from data socket fd.\n");
			return 1;
		}

		if ((bytes = fwrite(buf, bytes, 1, file)) < 0) {
			printf("ERROR: Cannot write data in file.\n");
			return 1;
		}
	}

	fclose(file);
	close(data_socket_fd);

	return 0;
}
