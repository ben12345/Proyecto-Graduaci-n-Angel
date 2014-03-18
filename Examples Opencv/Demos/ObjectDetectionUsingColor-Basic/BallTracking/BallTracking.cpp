﻿//#include "stdafx.h"
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

void createTrackbars();

 
int main(){
  
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
	    
	    /*Smooths the image in one of several ways*/
	    /*void cvSmooth(const CvArr* src, CvArr* dst, int smoothtype=CV_GAUSSIAN,
	     * int size1=3, int size2=0, double sigma1=0, double sigma2=0 )
	     * CV_GAUSSIAN linear convolution with a size1 x size2 Gaussian kernel*/
            cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel
	    /*Creates an image header and allocates the image data*/
	    /* cvGetSize: The function returns number of rows (CvSize::height)*/
	    /* and number of columns (CvSize::width) of the input matrix or image.*/
            IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
	    /*Converts an image from one color space to another.
	     *Change the color format from BGR to HSV*/
            cvCvtColor(frame, imgHSV, CV_BGR2HSV); 
	    /*This function threshold the HSV image and create a binary image*/
            IplImage* imgThresh = GetThresholdedImage(imgHSV);
	    
	    /*smooth the binary image using Gaussian kernel*/
	    cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); 
	    
	    /*Convert IplImage to Mat for detection of circles*/
	    imgThreshMat=cvarrToMat(imgThresh,false,true,0);
	    
	    vector<Vec3f> circles;

	    // Apply the Hough Transform to find the circles
	    HoughCircles( imgThreshMat, circles, CV_HOUGH_GRADIENT, 1, imgThreshMat.rows/8, 200, 100, 0, 1000 );

	    // Draw the circles detected
	    for( size_t i = 0; i < circles.size(); i++ )
	    {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle( imgThreshMat, center, 3, Scalar(0,255,0), -1, 8, 0 );
		// circle outline
		circle( imgThreshMat, center, radius, Scalar(0,0,255), 3, 8, 0 );
	    }
		      
            cvShowImage("Ball",imgThresh);           
            cvShowImage("Video",frame);
	    imshow( "Hough Circle Transform Demo", imgThreshMat);
	    
           
            //Clean up used images
            cvReleaseImage(&imgHSV);
            cvReleaseImage(&imgThresh);            
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

//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV){
	/*Creates an image header and allocates the image data*/
	/* cvGetSize: The function returns number of rows (CvSize::height)*( 
	/* and number of columns (CvSize::width) of the input matrix or image.*/
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
       
	/* Checks if array elements lie between the elements of two other arrays.	*/
	/* void cvInRangeS(const CvArr* src, CvScalar lower, CvScalar upper, CvArr* dst)*/
	/* src – first input array.							*/
	/* lowerb – inclusive lower boundary array or a scalar.				*/
	/* upperb – inclusive upper boundary array or a scalar.				*/
	/* dst – output array of the same size as src and CV_8U type.			*/
       //cvInRangeS(imgHSV, cvScalar(170,160,60), cvScalar(180,2556,256), imgThresh);
       cvInRangeS(imgHSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX), imgThresh); 
       return imgThresh;
}

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