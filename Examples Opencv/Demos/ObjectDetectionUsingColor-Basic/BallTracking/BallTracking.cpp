#include "stdafx.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

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
       cvInRangeS(imgHSV, cvScalar(100, 100, 100), cvScalar(110, 255, 255), imgThresh); 
       return imgThresh;
} 
 
int main(){
  
      CvCapture* capture =0;       
      capture = cvCaptureFromCAM(0);
      if(!capture){
			printf("Capture failure\n");
			return 0;
      }
      
      IplImage* frame=0;  
      IplImage* sample_1=0; //Variable de prueba
      
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
          
             //cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
            
            cvShowImage("Ball",imgThresh);           
            cvShowImage("Video",frame);
	    
           
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