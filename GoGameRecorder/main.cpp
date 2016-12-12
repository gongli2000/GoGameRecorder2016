#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "perspectiveTransform.hpp"
#include "drawing.hpp"

using namespace cv;
using namespace std;


void thresh_callback(int, void* );
int testContours( Mat& src );
void capturecamera();
void dothresh(Mat& src, int threshold);
void capturecontours();
Mat getoneframe();
void findlargestcontour(Mat& src);
int maxcontourarea(vector<vector<Point>> &contours);
void loopForBoundingRect();
void blobdetector();

int main(int, char**)
{
//    Mat src = imread("/Users/larry/sampleimages/lenna.png", 1);
   loopForBoundingRect();
//    blobdetector();
    return 0;
}



void loopForBoundingRect()
{
    VideoCapture videocam(0);
    if(!videocam.isOpened())return;
   
    Mat map;
    vector<Point> boundingPoly;
    bool nomap = true, loop = true, orient=false;
    int delta = 25, boardsize = 19, orientation= 2;
    Scalar color(0,0,255);
    namedWindow( "warped",CV_WINDOW_AUTOSIZE);
    namedWindow( "Contours",CV_WINDOW_AUTOSIZE);
    
    // Loop forever capturing frames from camera
    for(;;){
        Mat cframe;
        videocam >> cframe;
    
        // Find bounding polygon of grid lines of board and
        // find perspective transform that takes that polygon to a rectangle
        if(loop || nomap ){
            if(!orient){
                boundingPoly = tryToGetBoundingRectOfBoard(videocam);
            }
            vector<Point2f> src =srcRect(boundingPoly, orientation);
            vector<Point2f> dst = destRect(cframe.size(), delta);
            map =getPerspectiveTransform(src,dst);
    
            nomap = false;
            orient=false;
        }
        
        // Apply perspective transform to input image and get mappedImage
        Mat mappedImage = cframe.clone();
        warpPerspective(cframe, mappedImage, map, cframe.size());
        
        // Draw the bounding polygon of grid lines on input image and show it
        drawpoly(cframe, boundingPoly, color, 10);
        resize(cframe, cframe, Size(cframe.cols/2.5, cframe.rows/2.5));
        imshow( "Contours", cframe );
        moveWindow("Contours", 30, 30);
    
    
        // Draw grid lines on transformed image and show it.
        drawgrid(mappedImage, delta, boardsize, Scalar(0,255,0), 3);
        resize(mappedImage, mappedImage, Size(mappedImage.cols/2.7, mappedImage.rows/2));
        imshow("warped", mappedImage);
        moveWindow("warped",100 + cframe.cols, 30);
        
        
        switch(waitKey(1)){
            case 'c': nomap=true; break;
            case 'l': loop = !loop; break;
            case '1': orientation = 0;orient=true;nomap=true;break;
            case '2': orientation = 1;orient=true;nomap=true;break;
            case '3': orientation = 2;orient=true;nomap=true;break;
            case '4': orientation = 3;orient=true;nomap=true;break;
                
        }
        
    }
}

