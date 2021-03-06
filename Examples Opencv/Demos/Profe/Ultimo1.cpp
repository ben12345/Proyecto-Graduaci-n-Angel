#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
using namespace cv;


int H_MIN = 0;
int H_MAX = 255;
int S_MIN = 0;
int S_MAX = 255;
int V_MIN = 0;
int V_MAX = 255;

int lowThreshold = 1500;
int highThreshold = 2000;
int kernel_size = 5;

//extern int hue;			// This variable is adjusted by the user's trackbar at runtime.
//extern int saturation;	//		"
//extern int brightness;	//		"
//extern void colores();

int limit;

/*const string trackbarWindowName = "Trackbars";
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
	sprintf( TrackbarName, "lowThreshold", lowThreshold);
	sprintf( TrackbarName, "highThreshold", highThreshold);

	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
	createTrackbar( "highThreshold", trackbarWindowName, &highThreshold, highThreshold, on_trackbar );
	createTrackbar( "lowThreshold", trackbarWindowName, &lowThreshold, lowThreshold, on_trackbar );

	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
	createTrackbar( "highThreshold", trackbarWindowName, &highThreshold, highThreshold, on_trackbar );
	createTrackbar( "lowThreshold", trackbarWindowName, &lowThreshold, lowThreshold, on_trackbar );

}*/
cv::Mat showHistogram(const cv::Mat &inImage){
 
    cv::MatND hist;
    // For a gray scale [0:255] we have 256 bins
    const int bins[1] = {256};
    const float hranges[2] = {0.0, 255.0};
    const float* ranges[1] = { hranges };
    const int channels[1] = {1};
 
    cv::calcHist(&inImage,
            1,             // histogram from 1 image only
            channels,
            cv::Mat(),     // no mask is used
            hist,            // the output histogram
            1,             // 1D histogram
            bins,
            ranges         // pixel value ranges
    );
 
    // Get min and max bin values
    double maxVal=0;
    double minVal=0;
    cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);
    // The image to display the histogram
    cv::Mat histImg(bins[0], bins[0], CV_8U, cv::Scalar(255));
 
    // Map the highest point to 95% of the histogram height to leave some
    // empty space at the top
    const int histHeight = bins[0];
    const int maxHeight = 0.95 * histHeight;
 
    cv::Mat_<float>::iterator it    = hist.begin<float>();
    cv::Mat_<float>::iterator itend = hist.end<float>();
 
    int barPosition = 0;
    for ( ; it != itend; ++it) {
        float histValue = (*it);
        int barHeight = ( histValue * maxHeight ) / maxVal;
        cv::line(histImg,
                // start the line from the bottom, and go up based on the barHeight
                // Remember the (0,0) is the top left corner
                cv::Point(barPosition, histHeight),
                cv::Point(barPosition, histHeight - barHeight),
                cv::Scalar::all(0));
        barPosition++;
    }
 
    return histImg;
}

//This function threshold the HSV image and create a binary image 
int main(){
      CvCapture* capture;
      Mat frame,imgHSV,imgGRAY,imgThresh;
      capture = cvCaptureFromCAM(1);
      
      if(!capture){
			printf("Capture failure\n");
			return -1;
      }
      //createTrackbars();
      //colores();
      cvNamedWindow("Video");     
         //iterate through each frames of the video     
      while(true){

	frame = cvQueryFrame(capture);           
        
	if(frame.empty()) break;

	GaussianBlur(frame,frame,Size(5,5),0,0,BORDER_DEFAULT);
	GaussianBlur(frame,frame,Size(5,5),0,0,BORDER_DEFAULT);
	
	cvtColor(frame, imgHSV, CV_BGR2HSV);

	cvtColor(frame, imgGRAY, CV_BGR2GRAY);
	
	//inRange(imgHSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX), imgThresh);
	
	Mat histgram = showHistogram(imgHSV);
        
	inRange(imgHSV,Scalar(0,114,205),Scalar(242,255,255), imgThresh);
	
	imgGRAY&=imgThresh;
	
	Canny( imgGRAY, imgGRAY, lowThreshold, highThreshold, kernel_size );
	
        vector<Vec3f> circles;	
        HoughCircles(imgGRAY, circles, CV_HOUGH_GRADIENT,2, imgGRAY.rows/4, 100, 110, 10,150);
	
	if(circles.size()>4) limit = 4;
	else limit = circles.size();
	
	for( size_t i = 0; i < limit; i++ )
	{
	  Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	  int radius = cvRound(circles[i][2]);
	  rectangle(frame, cvPoint(circles[i][0] - radius,circles[i][1] - radius), cvPoint(circles[i][0] + radius,circles[i][1] + radius), Scalar( 0, 255, 0 ),2, 0);
	  //circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
	  putText(frame, "Ball", cvPoint(circles[i][0]+radius,circles[i][1]+radius),FONT_HERSHEY_COMPLEX_SMALL, 2, cvScalar(0,0,250), 1, CV_AA);
	}
   
        //imshow("Ball", imgGRAY);           
	imshow("Video", frame);
	imshow("Histogram", histgram);
          
        //Wait 50mS
        int c = cvWaitKey(1);
        //If 'ESC' is pressed, break the loop
        if((char)c==27 ) break;      
      }

      cvDestroyAllWindows() ;
      cvReleaseCapture(&capture);     

      return 0;
}
