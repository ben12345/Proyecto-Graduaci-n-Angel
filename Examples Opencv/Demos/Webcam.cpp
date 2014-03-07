#include "opencv2/opencv.hpp"
using namespace cv;
/// Function headers
int display_caption( const char* caption );
int display_dst( int delay );

int main(int, char**)
{
    int i=1;
    int max=100;
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat pict1,pict2,pict3;
    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        
       if(i>max) i=1;
        /// Create a Trackbar for user to enter threshold
	GaussianBlur(frame, pict1, Size(i,i),1,1);
	blur( frame, pict2, Size(i,i), Point(-1,-1) );
	medianBlur ( frame, pict3,i);
	imshow("Gaussian Filter", pict1);
        imshow("Normal Filter", pict2);
	imshow("Median Filter", pict3);
	imshow("Original", frame);
	i=i+2;
        if(waitKey(50) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
