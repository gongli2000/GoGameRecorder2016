//
//  perspectiveTransform.cpp
//  opencvplayground
//
//  Created by larry on 12/10/16.
//  Copyright © 2016 larry. All rights reserved.
//

#include "perspectiveTransform.hpp"


// Return a perspective transform from srcVec
// which is polygon of points of bounding rect of board
// to destination rect which just a rect of size s
Mat getPerspectiveMap(Size s, vector<cv::Point> &srcVec, int orientation,int dx)
{
    vector<Point2f> dst ={
        Point2f(dx,dx),
        Point(s.width-dx,dx),
        Point(s.width-dx,s.height-dx),
        Point(dx,s.height-dx)};
    
    vector<int>  botIndices {0,1,2,3} ;
    vector<int> rightIndices {1,2,3,0};
    vector<int> leftIndices {2,3,0,1};
    vector<int> topIndices {3,0,1,2};
    vector< vector<int> > indicesArray = {rightIndices,botIndices,leftIndices,topIndices};
    vector<int> indices = indicesArray[orientation];


    vector<Point2f> src ={srcVec[indices[0]],srcVec[indices[1]],srcVec[indices[2]],srcVec[indices[3]]};
    return getPerspectiveTransform(src,dst);
}

vector<Point2f> destRect(Size s, int delta)
{
    vector<Point2f> rect {
        Point2f(delta,delta),
        Point2f(s.width-delta,delta),
        Point2f(s.width-delta,s.height-delta),
        Point2f(delta,s.height-delta)};
    return rect;
}

vector<Point2f> srcRect(vector<Point>& r, int orientation)
{
    vector<int>  bot {0,1,2,3} ;
    vector<int> right {1,2,3,0};
    vector<int> left {2,3,0,1};
    vector<int> top {3,0,1,2};
    vector< vector<int> > indicesArray = {right,bot,left,top};
    
    vector<int> indx = indicesArray[orientation];
    vector<Point2f> src {r[indx[0]],r[indx[1]],r[indx[2]],r[indx[3]]};
    return src;
}


int maxcontourarea(vector<vector<Point>> &contours)
{
    int largest_area=0;
    int largest_contour_index=0;
    Rect bounding_rect;
    for( int i = 0; i< contours.size(); i++ )
    {
        double a=contourArea( contours[i],false);
        if(a>largest_area){
            largest_area=a;
            largest_contour_index=i;
            bounding_rect=boundingRect(contours[i]);
        }
    }
    return largest_contour_index;
}

vector<Point> getBoundingRectOfBoard(Mat &frame){
    Mat canny_output,srcgray;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Point> hull;
    Mat edges;
    
    // Find edges of blurred black and white image
    cvtColor(frame, edges, COLOR_BGR2GRAY);
    GaussianBlur(edges, edges, Size(11,11), 1.5, 1.5);
    Canny(edges, canny_output, 0, 30, 3);
  
    // Get contours from edges
    findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
 
    // save the contour with max area as candidate for bounding rect
    int max_area_index = maxcontourarea(contours);
    convexHull( Mat(contours[max_area_index]), hull, false );
    return hull;
}

vector<Point>  tryToGetBoundingRectOfBoard(VideoCapture& cap,int ntries, int maxtimes){
    vector<vector<Point>>  hulls;
    vector<Point> boundingPoly;
    Mat frame;
    // Try to find bounding rect contour ntries times
    // return the contour
    int n=0,ntimes=0;
    while(n < ntries && (ntimes < maxtimes)){
        if(cap.read(frame)){
            vector<Point> boundingPoly;
            vector<Point> r = getBoundingRectOfBoard(frame);
            double perimeter_length = cv::arcLength(r,true);
            approxPolyDP( r, boundingPoly, 0.01*perimeter_length, true );
            if(boundingPoly.size() == 4){
                hulls.push_back(boundingPoly);
                n++;
            }
        }

    }
    // of all the candidates for bounding rect get the one with max area
    if(hulls.size() > 0){
        int i =maxcontourarea(hulls);
        return hulls[i];
    }else{
        return vector<Point>();
    }
}
