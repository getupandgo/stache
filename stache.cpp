/**
 * Based on https://code.ros.org/trac/opencv/browser/trunk/opencv/samples/cpp/tutorial_code/objectDetection/objectDetection2.cpp?rev=6553
 */
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay(Mat frame);
void saveFrame(Mat frame);
int inputAvailable();
void inputSetup(int setup);
void changeStache();

/** Global variables */
String face_cascade_name = "lbpcascade_frontalface.xml";
CascadeClassifier face_cascade;
const char * window_name = "stache - OpenCV demo";
IplImage* mask = 0;
IplImage* glasses = 0;
IplImage* hat = 0;

/** Command-line arguments */
int numCamera = -1;
const char* stacheMaskFile = "stache-mask.png";
const char* glassMaskFile = "glass-mask.png";
const char* hatMaskFile = "hat-mask.png";
int camWidth = 0;
int camHeight = 0;
float camFPS = 0;
int currentStache = 0;
int stacheCount = 0;
const char **stacheFilenames;

/**
 * @function main
 */
int main(int argc, const char** argv) {
  CvCapture* capture;
  Mat frame;
  int c;

  if(argc > 1) {
    stacheMaskFile = argv[1];
    stacheCount = argc - 2;
    stacheFilenames = argv + 1;
  }

  //-- 1. Load the cascade
  if( !face_cascade.load(face_cascade_name) ){ fprintf(stderr, "--(!)Error loading\n"); exit(-1); };

  //-- 1a. Load the mustache mask
  mask = cvLoadImage(stacheMaskFile);
  if(!mask) { fprintf(stderr, "Could not load %s\n", stacheMaskFile); exit(-1); }

  glasses = cvLoadImage(glassMaskFile);
  if(!glasses) { fprintf(stderr, "Could not load %s\n", glassMaskFile); }

  hat = cvLoadImage(hatMaskFile);
  if(!hat) { fprintf(stderr, "Could not load %s\n", hatMaskFile); exit(-1); }

  //-- 2. Read the video stream
  capture = cvCaptureFromCAM(numCamera);
  //if(camWidth) cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, camWidth);
  //if(camHeight) cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, camHeight);
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 320);
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
  //if(camFPS) cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, camFPS);
  if(capture) {
    inputSetup(1);
    while(true) {
      frame = cvQueryFrame(capture);
  
      //-- 3. Apply the classifier to the frame
      try {
       if(!frame.empty()) {
        detectAndDisplay( frame );
       } else {
        fprintf(stderr, " --(!) No captured frame!\n");
       }
      } catch(...) {
      }
    }
    inputSetup(0);
  } else {
    exit(-3);
  }
  exit(0);
}

/**
 * @function detectAndDisplay
 */
void detectAndDisplay(Mat frame) {
  std::vector<Rect> faces;
  Mat frame_gray;
  int i = 0;
  int c;

  cvtColor(frame, frame_gray, CV_BGR2GRAY);
  equalizeHist(frame_gray, frame_gray);

  //-- Detect faces
  face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0, Size(80, 80));

  for(; i < faces.size(); i++) {
    //-- Scale and apply mustache mask for each face
    Mat faceROI = frame_gray(faces[i]);
    IplImage iplFrame = frame;
    int height = faces[i].height; // *4/3;
    int offset = faces[i].y; // - faces[i].height/4;
    if(offset < 0) offset = 0;

    IplImage *iplMask = cvCreateImage(cvSize(faces[i].width, height), mask->depth, mask->nChannels );
    cvSetImageROI(&iplFrame, cvRect(faces[i].x, offset, faces[i].width, height));
    cvResize(mask, iplMask, CV_INTER_LINEAR);
    cvSub(&iplFrame, iplMask, &iplFrame);
    cvResetImageROI(&iplFrame);

    IplImage *iplGlass = cvCreateImage(cvSize(faces[i].width, height), glasses->depth, glasses->nChannels );
    cvSetImageROI(&iplFrame, cvRect(faces[i].x, offset, faces[i].width, height));
    cvResize(glasses, iplGlass, CV_INTER_LINEAR);
    cvSub(&iplFrame, iplGlass, &iplFrame);
    cvResetImageROI(&iplFrame);

//    IplImage *iplHat = cvCreateImage(cvSize(faces[i].width, height), hat->depth, hat->nChannels );
//    cvSetImageROI(&iplFrame, cvRect(faces[i].x, offset, faces[i].width, height * 1/3));
//    cvResize(hat, iplHat, CV_INTER_LINEAR);
//    cvSub(&iplFrame, iplHat, &iplFrame);
//    cvResetImageROI(&iplFrame);
  }

  if(i>0) {
    c = waitKey(10);
    //c = inputAvailable();
    //if( c > 0 ) fprintf(stderr, "Got key %d.\n", c);
    if( c == 65361 || c == 63234 ) { saveFrame(frame); }  //-- save on press of left arrow
    if( c == 65363 || c == 63235 ) { changeStache(); }  //-- change stache on press of right arrow
  }

  //-- Show what you got
  flip(frame, frame, 1);
  imshow(window_name, frame);

  //-- 0a. Attempt to resize window
  //cvResizeWindow(window_name, camWidth, camHeight);
}

void changeStache() {
  while(stacheCount > 0) {
    if(currentStache < stacheCount) {
      currentStache++;
    } else {
      currentStache = 0;
    }
    stacheMaskFile = stacheFilenames[currentStache];
    mask = cvLoadImage(stacheMaskFile);
    if(mask) break;
    fprintf(stderr, "Could not load %s\n", stacheMaskFile);
  }
}

void saveFrame(Mat frame) {
  char filename[40];
  time_t mytime;
  struct tm y2k;
  y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;
  IplImage iplFrame = frame;
  time(&mytime);
  sprintf(filename, "tmp/captured%010ld.jpg", (long)difftime(mytime,mktime(&y2k)));
  cvSaveImage(filename, &iplFrame);
  fprintf(stdout, "{\"tweet\":\"New BeagleStache captured!\",\"filename\":\"%s\"}\n", filename);
  fflush(stdout);
}

int inputAvailable()  
{
  struct timeval tv;
  int c;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  if(FD_ISSET(0, &fds)) {
   c = getchar();
   return c;
  } else {
   return 0;
  }
}

void inputSetup(int setup)  
{
  static struct termios oldt, newt;
  if(setup) {
   tcgetattr(STDIN_FILENO, &oldt);
   newt = oldt;
   newt.c_lflag &= ~(ICANON);
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  } else {
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
  }
}
