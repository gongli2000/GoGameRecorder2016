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
    Point2f dst[]={
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


    Point2f src[]={srcVec[indices[0]],srcVec[indices[1]],srcVec[indices[2]],srcVec[indices[3]]};
    return cv::getPerspectiveTransform(src,dst);
}

Mat getInvPerspectiveMap(Size s, vector<cv::Point> &destVec, int orientation,int dx)
{
    Point2f src[]={
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
    
    
    Point2f dst[]={destVec[indices[0]],destVec[indices[1]],destVec[indices[2]],destVec[indices[3]]};
    return cv::getPerspectiveTransform(src,dst);
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

vector<Point>  tryToGetBoundingRectOfBoard(VideoCapture& cap){
    int ntries = 6;
    vector<vector<Point>>  hulls;
    Mat frame;
    // Try to find bounding rect contour ntries times
    // return the contour
    for(int cnt =0; cnt < ntries; cnt++){
        
        if(cap.read(frame)){
            hulls.push_back( getBoundingRectOfBoard(frame));
        }
    }
    // of all the candidates for bounding rect get the one with max area
    int i =maxcontourarea(hulls);
    
    // approximate the contour with a polygon hopefully with 4 vertices
    vector<Point> boundingPoly;
    double perimeter_length = cv::arcLength(hulls[i],true);
    cv:: approxPolyDP( hulls[i], boundingPoly, 0.01*perimeter_length, true );
    

    return boundingPoly;
}
