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

/* FTP WORKS ON APPLICATION LAYER! MAIN GOAL: MOVING FILES ( in our project we are supposed to only download a file)
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

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
