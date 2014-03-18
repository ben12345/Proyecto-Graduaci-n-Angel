//#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
using namespace cv;

int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
const string trackbarWindowName = "Trackbars";
void on_trackbar( int, void* ){}

void createTrackbars(){
	//create window for trackbars


    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}

 
int main(int argc, char* argv[]){
  
      CvCapture* capture =0;       
      capture = cvCaptureFromCAM(0);
      if(!capture){
			printf("Capture failure\n");
			return 0;
      }
      
      IplImage* frame=0;  
      IplImage* sample_1=0; //Variable de prueba
      Mat imgThreshMat;
      createTrackbars();
      cvNamedWindow("Video");     
      cvNamedWindow("Ball");


      //iterate through each frames of the video     
      while(true){
	    /*Grabs, decodes and returns the next video frame.*/
            frame = cvQueryFrame(capture);           
            if(!frame) break;
	    /*Makes a full copy of an image, including the header, data, and ROI.*/
            frame=cvCloneImage(frame); 
	    
            cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel
	   
            IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
	    
            cvCvtColor(frame, imgHSV, CV_BGR2HSV); 
	
	    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);

	    cvInRangeS(imgHSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX), imgThresh);
	    
	    cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); 
	   		      
            cvShowImage("Ball",imgThresh);  
         
            cvShowImage("Video",frame);
	    
           
            //Clean up used images
            cvReleaseImage(&imgHSV);
            //cvReleaseImage(&imgThresh);            
            cvReleaseImage(&frame);

            //Wait 50mS
            int c = cvWaitKey(10);
            //If 'ESC' is pressed, break the loop
            if((char)c==27 ) break;      
      }

      cvDestroyAllWindows() ;
      cvReleaseCapture(&capture);     

      return 0;
}

IplImage* GetThresholdedImage(IplImage* imgHSV){
	
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
       
       cvInRangeS(imgHSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX), imgThresh); 

       return imgThresh;
}

