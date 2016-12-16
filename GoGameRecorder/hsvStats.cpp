//
//  hsvStats.cpp
//  GoGameRecorder
//
//  Created by larry on 12/15/16.
//  Copyright © 2016 larry. All rights reserved.
//

#include "hsvStats.hpp"


void getHSVstats(){
    VideoCapture videocam(0);
    if(!videocam.isOpened())return;
    
    Mat img;
    videocam >> img;
}


void plothistogram(string windowname,Mat& hist,int histsize){
    // Plot the histogram
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histsize );
    
    Mat histImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    
    for( int i = 1; i < histsize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
    }
    
    namedWindow( windowname, 1 );
    resize(histImage,histImage,Size(300,300));
    imshow( windowname, histImage );
}

void Get_Threshold_Values()
{
    VideoCapture videocam(0);
    if(!videocam.isOpened())return;
    
    for(;;){
    Mat gray;
    videocam >> gray;
    namedWindow( "Gray", 1 );
  
    cvtColor(gray, gray, CV_BGR2HSV);
   
    
    // Initialize parameters
    vector<int> histSize = {256};    // bin size
    vector<float> range = { 0.0, 255.0 };
    vector<float> ranges = {  0.0, 255.0  };
    
    // Calculate histogram
   
    vector<Mat> images = {gray};
    
    for(int i =0;i<3;i++){
        Mat hist;
        vector<int> channels = {i};
        calcHist(images,channels,Mat(),hist,histSize,ranges);
        char s[256];
        sprintf(s, "histogram_%d", i);
        plothistogram(s,hist,histSize[0]);
    }
    resize(gray,gray,Size(300,300));
    imshow( "Gray", gray );
     waitKey(1);
    }
   
}
