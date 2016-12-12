//
//  tests.cpp
//  opencvplayground
//
//  Created by larry on 12/10/16.
//  Copyright © 2016 larry. All rights reserved.
//

#include "tests.hpp"

using namespace cv;
using namespace std;
Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
void thresh_callback(int, void* );



void blobdetector(){
    
    Mat im = imread( "/Users/larry/Documents/goboard.png", IMREAD_GRAYSCALE );
    
    
    GaussianBlur(im, im, Size(3,3), 1.5, 1.5);
    
    
    SimpleBlobDetector::Params params;
    
    // Change thresholds
    //    params.minThreshold = 10;
    //    params.maxThreshold = 200;
    
    // Filter by Area.
    //    params.filterByArea = true;
    //    params.minArea = 1500;
    //
    // Filter by Circularity
    //    params.filterByCircularity = true;
    //    params.minCircularity = 0.1;
    //
    // Filter by Convexity
    params.filterByConvexity = true;
    params.minConvexity = 0.87;
    
    // Filter by Inertia
    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;
    
    // Set up detector with params
    SimpleBlobDetector detector(params);
    
    
    // Detect blobs.
    std::vector<KeyPoint> keypoints;
    detector.detect(im, keypoints);
    
    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
    Mat im_with_keypoints;
    drawKeypoints( im, keypoints, im_with_keypoints,
                  Scalar(0,0,255),
                  DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
    
    // Show blobs
    imshow("keypoints", im_with_keypoints );
    waitKey(0);
}

void capturecamera()
{
    VideoCapture cap(0);
    if(!cap.isOpened())return;
    
    Mat edges;
    namedWindow("edges",1);
    for(;;){
        Mat frame;
        cap >> frame;
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
        if(waitKey(30) >= 0) break;
    }
}


int testContours( Mat& src )
{
 
    cvtColor( src, src_gray, CV_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );
    
    /// Create Window
    string source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, src );
    
    createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
    thresh_callback( 0, 0 );
    
    // waitKey(0);
    return(0);
}

void dothresh(Mat& src, int threshold)
{
    Mat canny_output,srcgray;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    cvtColor( src, srcgray, CV_BGR2GRAY );
    blur( srcgray, srcgray, Size(3,3) );
    Canny( srcgray, canny_output, threshold, threshold*2, 3 );
    
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    
    /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
    waitKey(1);
}





void thresh_callback(int, void* )
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    /// Detect edges using canny
    Canny( src_gray, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    /// Draw contours
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    
    /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
}

void findlargestcontour(Mat& src){
    {
        int largest_area=0;
        int largest_contour_index=0;
        Rect bounding_rect;
        
        Mat thr(src.rows,src.cols,CV_8UC1);
        Mat dst(src.rows,src.cols,CV_8UC1,Scalar::all(0));
        cvtColor(src,thr,CV_BGR2GRAY); //Convert to gray
        threshold(thr, thr,25, 255,THRESH_BINARY); //Threshold the gray
        
        vector<vector<Point>> contours; // Vector for storing contour
        vector<Vec4i> hierarchy;
        
        findContours( thr, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
        
        for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
        {
            double a=contourArea( contours[i],false);  //  Find the area of contour
            if(a>largest_area){
                largest_area=a;
                largest_contour_index=i;                //Store the index of largest contour
                bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
            }
            
        }
        
        Scalar color( 255,255,255);
        drawContours( dst, contours,largest_contour_index, color, CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
        rectangle(src, bounding_rect,  Scalar(0,255,0),1, 8,0);
        imshow( "src", src );
        imshow( "largest Contour", dst );
        waitKey(0);
    }
    
}
