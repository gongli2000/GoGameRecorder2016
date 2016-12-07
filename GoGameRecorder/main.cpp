
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 50;
int ratio = 3;
int kernel_size = 3;
string window_name = "Edge Map";

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
    /// Reduce noise with a kernel 3x3
    blur( src_gray, detected_edges, Size(3,3) );
    
    /// Canny detector
    int lt = lowThreshold+20;
    Canny( detected_edges, detected_edges, lt, lt*ratio, kernel_size );
    
    /// Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);
    
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}

// update
// adsfdsf
/** @function main */
int main( int argc, char** argv )
{
    /// Create a window
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );
    
    /// Create a Trackbar for user to enter threshold
    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
    
    
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    
    for(;;)
    {
        //Mat frame;
        cap >> src; // get a new frame from camera
        
        /// Create a matrix of the same type and size as src (for dst)
        dst.create( src.size(), src.type() );
        
        /// Convert the image to grayscale
        cvtColor( src, src_gray, CV_BGR2GRAY );
        
        
        /// Show the image
        CannyThreshold(0, 0);
        
        if(waitKey(30) >= 0) break;
    }
    
    
    
    
    return 0;
}
