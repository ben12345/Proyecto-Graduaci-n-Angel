﻿
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
	                                
	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}


int main(int argc, char* argv[]){
  
      CvCapture* capture;
      Mat frame,imgHSV,imgGRAY,imgThresh;
      capture = cvCaptureFromCAM(0);
      
      if(!capture){
			printf("Capture failure\n");
			return -1;
      }
      createTrackbars();
      cvNamedWindow("Video"); 

      //iterate through each frames of the video     
      while(true){
	    /*Grabs, decodes and returns the next video frame.*/
            frame = cvQueryFrame(capture); 
	    
	    
            if(frame.empty()) break;

	    GaussianBlur(frame,frame,Size(9,9),1,1,BORDER_DEFAULT);

	    cvtColor(frame, imgHSV, CV_BGR2HSV);
	    
	    cvtColor(frame, imgGRAY, CV_BGR2GRAY);
	 
	    inRange(imgHSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX), imgThresh);
	
	    GaussianBlur(imgThresh,imgThresh,Size(3,3),1,1,BORDER_DEFAULT);
	    
	    vector<Vec3f> circles;	    	    
	    Mat drawing = Mat::zeros( imgThresh.size(), CV_8UC3 );
	   
	    vector<vector<Point> > contours;
	    vector<Vec4i> hierarchy;
	    	    
	    /*findContours( imgThresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	    
	    /// Get the moments
	    
	    vector<Moments> mu(contours.size());
	    
	    for( int i = 0; i < contours.size(); i++ )
	      { 
		mu[i] = moments( contours[i], false ); 
		
	      }
	      
	    ///  Get the mass centers:
	    vector<Point2f> mc( contours.size() );
	    for( int i = 0; i < contours.size(); i++ )
	      { 
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	      }

	    /// Draw contours
	    for( int i = 0; i< contours.size(); i++ )
	      {
		Scalar color = Scalar(255, 255, 255);
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
		circle( drawing, mc[i], 4, color, -1, 8, 0 );
	      } */ 
	    
	    HoughCircles(imgGRAY, circles, CV_HOUGH_GRADIENT,2, imgGRAY.rows/4, 200, 100);
	    
	    for( size_t i = 0; i < circles.size(); i++ )
	      {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
	      }
	    
            //cvShowImage("Ball",imgThresh);
         
            imshow("Video",frame);
	    imshow( "Contours", imgThresh );

            //Wait 50mS
            int c = cvWaitKey(1);
            //If 'ESC' is pressed, break the loop
            if((char)c==27 ) break;      
      }

      cvDestroyAllWindows() ;
      cvReleaseCapture(&capture);     

      return 0;
}

