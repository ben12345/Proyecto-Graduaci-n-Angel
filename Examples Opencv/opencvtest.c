//#include
#include <opencv2/highgui/highgui.hpp>
 
int main()
{
   IplImage* img = cvLoadImage("/home/aphillips/Documents/Examples Opencv/python.jpg",CV_LOAD_IMAGE_COLOR);
   cvNamedWindow("opencvtest",CV_WINDOW_AUTOSIZE);
   cvShowImage("opencvtest",img);
   cvWaitKey(0);
   cvReleaseImage(&img);
   return 0;
}