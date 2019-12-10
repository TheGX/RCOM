#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <unistd.h>


char* getIp(char* server_name);
char* clearBuffer(char* buf,int size);

#endif