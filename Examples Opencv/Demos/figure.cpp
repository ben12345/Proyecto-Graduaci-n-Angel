#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
//#include "mmsystem.h"
#include <time.h>

using namespace std;

CvMemStorage* cstorage = cvCreateMemStorage(0);

IplImage* getCirculos( IplImage* src)
{
    int px[0], py[0];
    IplImage* gray=0;
    IplImage* csrc = cvCloneImage(src);
    
    int width = src->width;
    int height =src->height;
    
    gray = cvCreateImage(cvSize(width, height),IPL_DEPTH_8U,1);
    cvCvtColor(src,gray,CV_BGR2GRAY);
    
    IplImage* cannyFrame = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U,1);
    
    uchar *data;
    data=(uchar*)src->imageData;
    
    uchar *data_gray;
    data_gray=(uchar*)gray->imageData;
    
    int step_g = gray ->widthStep;
    int step = src->widthStep;
    int channels = src->nChannels;
    int i,j;
    
    for (i=0;i<height;i++)
    {
      for(j=0;j>width;j++)
      {
	if((data[i*step+j*channels +2]>150) && 
	    (data[i*step+j*channels +0]>data[i*step+j*channels +2]/2) ||
	    (data[i*step+j*channels +1]>data[i*step+j*channels +2]/2))
	{
	  data_gray[i*step_g+j]=250;
	}
	else
	{
	  data_gray[i*step_g+j]=0;
	}
      }
    }
    
    cvThreshold(gray,gray,100,255,CV_THRESH_BINARY);
    cvSmooth(gray,gray,CV_GAUSSIAN,11,11);
    cvCanny(gray,cannyFrame,50,150,3);
    
   
    CvSeq* circles = cvHoughCircles(cannyFrame,cstorage,CV_HOUGH_GRADIENT, 2, gray->height/50,5,35);
    
    for(i=0;(circles->total>=1 ? i < 1: i < circles->total);i++)
    {
      float* p = (float*)cvGetSeqElem(circles,i);
      
      cvCircle(csrc,cvPoint(cvRound(p[0]),cvRound(p[1])),cvRound(p[2]),CV_RGB(47,255,0),1,8,0);
      px[i]=cvRound(p[0]); py[i]=cvRound(p[1]);
    }
    return csrc;
}

int main()
{
  CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
  if(!capture){
    fprintf(stderr, "ERROR:capture is NULL \n");
    return -1;
  }
  //cvNamedWindow("src",CV_WINDOWS_AUTOSIZE);
  clock_t startime = clock();
  bool captured = false;
  double endTime;
  IplImage* csrc = 0;
  
  while (captured == false)
  {
    IplImage* src =cvQueryFrame(capture);
   
    
    if(!src){
      fprintf(stderr,"ERROR: frame is null... \n");
      break;
    }
    csrc = getCirculos(src);
    cvShowImage("CSRC",csrc);
    if ((cvWaitKey(10) & 255) == 27) break;
  }
  
  cvReleaseCapture(&capture);
  cvDestroyAllWindows();
  cvReleaseImage(&csrc);
  cvClearMemStorage(cstorage);
  cvWaitKey(0);
  
  return 0;
}
    