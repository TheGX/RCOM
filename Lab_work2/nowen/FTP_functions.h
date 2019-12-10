#ifndef _FUNCTION_FTP_H_
#define _FUNCTION_FTP_H_

#include "functions.h"

#define SERVER_PORT 21
#define SERVER_NAME "ftp-extrack.alwaysdata.net"

char* FTP_read(int socket_cmd,char* str,int size);
int FTP_write(int socket_cmd,char* buf,char* CMD,char* msg);
int FTP_connect(char* server_name, int server_port);
int* FTP_Mode_PASV(int socket_data,char* str);
int FTP_Download(int data_socket_fd, const char* filename);

#endif