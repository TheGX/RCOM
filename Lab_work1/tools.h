#ifndef TOOLS
#define TOOLS

#include <inttypes.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define SIZE_DATAPACKAGE 256// size of data packages = 255*256+255
#define SIZE_FRAME (SIZE_DATAPACKAGE+1)*2+5
#define TLV_N 2// name of file, size of file
#define FER 0 //Frame Error Rate em percentagem
#define TPROP 0 //in microseconds

//DATALINK LEVEL
#define FLAG 0x7E //0111 1110
#define FLAG_PPP 0x5E
#define ESC 0x7D
#define ESC_PPP 0x5D
#define A_S 0x03 //0000 0011
#define A_R 0x01 //0000 0001
#define C_SET 0x03 //0000 0011
#define C_UA 0x07 //0000 0111
#define C_DISC 0x0B //00001011
#define C_RR0  0x05  //0000 0101
#define C_REJ0 0x01 //0000 0001
#define C_RR1  0x85  //1000 0101
#define C_REJ1 0x81  //1000 0001
#define C_NS0 0x00 //0000 0000
#define C_NS1 0x40 //0100 0000

//APPLICATION LEVEL
#define AP_START 0x02 //0000 0010
#define AP_DATA 0x01 //0000 0001
#define AP_END 0x03 //0000 0011

#define MaxTries 3

#define MICRO 1000L
#define MILI 1000000L

extern int timeout;
extern int n_timeout;

/////////////////////////////////////////////////////
int fer_counter;         //counts REJs
uint64_t timeDatalink;
int byteS_counter;
//uint64_t bitRateTimer;  Ver sender() em printProgressBar call
struct timespec inito, finito;
/////////////////////////////////////////////////////


typedef struct {
  //unsigned char C;
	unsigned char T;
	unsigned char L;
	unsigned char *V;
} ControlPackage;

typedef struct {

  //unsigned char C;
	unsigned char N;
	unsigned char L1;
	unsigned char L2;
	unsigned char *file_data;
} DataPackage;

typedef enum {
	START_CONNECTION, FLAG_RCV, A_RCV, C_RCV, BCC_OK, STOP_CON
} connectionState;

void errorGenerator(unsigned char *buffer, int size);

uint64_t transform(struct timespec* aux);


int setPort(char *port, struct termios *oldtio);
int resetPort(int fd, struct termios *oldtio);

void buildConnectionFrame( unsigned char *connectionFrame, unsigned char A, unsigned char C);
int buildFrame( unsigned char * frame, int C_ns, unsigned char* message, int lenght);
unsigned char buildBCC2(unsigned char *message, int lenght);

int buildDataPackage(unsigned char* buffer, unsigned char* package, int size, int seq_n);
void rebuildDataPackage(unsigned char* packet, DataPackage *packet_data);

int buildControlPackage(unsigned char C, unsigned char* package, ControlPackage *tlv);
void rebuildControlPackage(unsigned char* package, ControlPackage *tlv);

int fileLenght(int fd);
char* connectionStateMachine(int fd);

int readFromPort(int fd, unsigned char* frame);


int stuffing (int length, unsigned char* buffer, unsigned char* frame, int frame_length, unsigned char BCC2);
int destuffing (int length, unsigned char* buffer, unsigned char* frame);
void printProgressBar(float current, float total);
#endif
