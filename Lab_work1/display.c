// #ifndef DISPLAY
// #define DISPLAY

#include <opencv2\highgui\highgui_c.h>
#include <highgui.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <inttypes.h>
#include <termios.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <inttypes.h>

#include "string.h"
#include "ApplicationLayer.h"
#include "tools.h"
#include "datalink.h"
#include "sender.h"
#include "receiver.h"


void appendGif(char* src, char* add) {

        int lenSrc = strlen(src);
        int lenAdd = strlen(add);
        strcat(src, add)
        src[len+add] = '\0';
}



void display(char * name){

  char buff[30];
  char gif[4]= ".gif";

  appendGif(name, gif);

  IplImage *im = cvLoadImage(name, CV_WINDOW_AUTOSIZE);

  cvNamedWindow(name);
  cvShowImage(name, im);
  cvWaitKey(0);
  cvReleaseImage(&im);
  cvDestroyAllWindows();

}

int main(int argc, char const *argv[]) {


  char buffer[420];

  if(!display(argv[1])){
    perror("u dun fucked up");
  }



  return 0;
}

// #endif
