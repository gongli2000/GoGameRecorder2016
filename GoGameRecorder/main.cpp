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
    VideoCapture cap(0);
    if(!cap.isOpened())return;
   
    Mat map;
    vector<Point> h;
    bool nomap = true;
    bool loop = true;
    bool orient=false;
    int delta = 25;
    int boardsize = 19;
    namedWindow( "warped",CV_WINDOW_AUTOSIZE);
    namedWindow( "Contours",CV_WINDOW_AUTOSIZE);
    int orientation = 2;
    Scalar color(0,0,255);

    for(;;){
        
        Mat cframe;
        cap >> cframe;
        
        if(loop || nomap ){
            if(!orient){
                h = tryToGetBoundingRectOfBoard(cap);
//                Mat temp = cframe.clone();
//                Mat temp2;
//                map = getPerspectiveMap(temp.size(), h,orientation,0);
//                warpPerspective(temp, temp2, map, temp.size());
//                vector<Point> g = getBoundingRectOfBoard(temp2);
//                
//                Mat invmap = cv::getPerspectiveTransform(g,h);
//                waitKey(0);
            }
            map = getPerspectiveMap(cframe.size(), h,orientation,delta);
            nomap = false;
            orient=false;
        }
        
        Mat mappedImage = cframe.clone();
        warpPerspective(cframe, mappedImage, map, cframe.size());
        
        drawpoly(cframe, h, color, 2);
        resize(cframe, cframe, Size(cframe.cols/2.5, cframe.rows/2.5));
        imshow( "Contours", cframe );
        moveWindow("Contours", 30, 30);
    
    
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

