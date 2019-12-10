#include "functions.h"
#include "FTP_functions.h"

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

char* clearBuffer(char* buf,int size){
	for(int i=0;i<size;i++){
		buf[i]=' ';
	}
	return buf;
}

char** parseur(char* string,char* separator,int* path_size){
    char** parsed;
    char *found;

    parsed = malloc(20*10*sizeof(char));

    printf("Original string: '%s'\n",string);

    found = strtok(string,separator);
    parsed[*path_size]=found;
    if( found==NULL){
        printf("\t'%s'\n",string);
        puts("\tNo separators found");
        return NULL;
    }
    while(found){
        printf("\tPath[%d]='%s'\n",*path_size,found);
        found = strtok(NULL,separator);
        parsed[*path_size+1]=found;
        printf("Path size = %d\n",*path_size);
        *path_size = *path_size+1;
    }
    return parsed;
}

void setURL(URL* url,char* input){
    int size_1    = 0;
	char** data_1 = parseur(input,":",&size_1);

    int size_2    = 0;
	char** data_2 = parseur(data_1[1],"/",&size_2);
    
    int size_3    = 0;
	char** data_3 = parseur(data_1[2],"@",&size_3);
    
    int size_4    = 0;
	char** data_4 = parseur(data_3[1],"/",&size_4);

    url->user=data_2[0];
    url->password=data_3[0];
    url->host=data_4[0];
    url->ip=getIp(url->host);
    url->port=21;
    url->filename=data_4[6];
    url->path=malloc(20*10*sizeof(char));

    for(int i =1;i<size_4-1;i++){
        url->path[i-1]=data_4[i];
        url->path_size=url->path_size+1;
    }
}

void printURL(URL url){
    printf("\n-------------URL-------------\n");
    printf("- User = %s\n",url.user);
    printf("- Pass = %s\n",url.password);
    printf("- Host = %s\n",url.host);
    printf("- Ip   = %s\n",url.ip);
    printf("- Port = %d\n",url.port);
    printf("- Path = ");
    for(int i=0;i<url.path_size;i++){
        printf("/%s",url.path[i]);
    }
    printf("\n- Size = %d\n",url.path_size);
    printf("- File = %s\n",url.filename);
    printf("------------------------------\n");
}