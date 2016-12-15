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
                boundingPoly = tryToGetBoundingRectOfBoard(videocam, 2,5);
                //boundingPoly = getBoundingRectOfBoard(cframe);
            }
            if(boundingPoly.size() == 0) continue;
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

        // Draw grid lines on transformed image and show it.
        drawgrid(mappedImage, delta, boardsize, Scalar(0,255,0), 3);

        
        // Find edges of blurred black and white image
        Mat edges,canny_output;
        cvtColor(cframe, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(11,11), 1.5, 1.5);
        Canny(edges, canny_output, 0, 30, 3);
        
        cvtColor(canny_output,canny_output,COLOR_GRAY2BGR);
        vector<Mat> images = {cframe,mappedImage,canny_output};
        Mat hout,hout2;
        Mat contours = cframe.clone();
        
        hconcat(cframe,mappedImage,hout);
        hconcat(canny_output,contours,hout2);
        
        Mat vout;
        vconcat(hout,hout2,vout);
        resize(vout,vout,Size(vout.cols/2.8,vout.rows/2.8));
        imshow("sdf",vout);
        
        
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

