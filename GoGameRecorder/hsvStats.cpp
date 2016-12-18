//
//  hsvStats.cpp
//  GoGameRecorder
//
//  Created by larry on 12/15/16.
//  Copyright © 2016 larry. All rights reserved.
//

#include "hsvStats.hpp"
#include "drawing.hpp"

void getHSVstats(){
    VideoCapture videocam(0);
    if(!videocam.isOpened())return;
    
    Mat img;
    videocam >> img;
}


Mat  plothistogram(Mat& hist,int histsize){
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
    return histImage;
}

void Get_Threshold_Values(VideoCapture &videocam)
{

    for(;;)
    {
        Mat gray,savegray;
        videocam >> gray;
        Mat blankimage = gray.clone().setTo(Scalar(255,255,255));
        savegray = gray.clone();
        namedWindow( "Gray", 1 );
      
        cvtColor(gray, gray, CV_BGR2HSV);
        vector<int> histSize = {256};    // bin size
        vector<float> range = { 0.0, 255.0 };
        vector<float> ranges = {  0.0, 255.0  };
        

        vector<Mat> inputimage = {gray},images;
        Scalar means,stddev;
        meanStdDev(gray, means,stddev);
        char minmean[256], maxmean[256];
    
    
        for(int i =0;i<3;i++){
            Mat hist, histout;
            vector<int> channels = {i};
            calcHist(inputimage,channels,Mat(),hist,histSize,ranges);
            histout =plothistogram(hist,histSize[0]);

            cvtColor(histout,histout,CV_GRAY2BGR);
            images.push_back(histout);
        }
        Mat threshold_frame;
        double delta = 20;
        Scalar minmeans = means - Scalar(delta,delta,delta);
        Scalar maxmeans = means + Scalar(delta,delta,delta);
        
//        minmeans = Scalar(0,0,-20);
//        maxmeans = Scalar(255,255,60);
        inRange(gray, minmeans, maxmeans, threshold_frame);
        cvtColor(threshold_frame,threshold_frame,CV_GRAY2BGR);
        
     
        sprintf(minmean,"min %.2f,  %.2f, %.2f\n",
                minmeans[0],minmeans[1],minmeans[2]);
        sprintf(maxmean,"max %.2f, %.2f , %.2f\n\n",
                maxmeans[0],maxmeans[1],maxmeans[2]);
        
        putText(blankimage, minmean, Point(50,100),FONT_HERSHEY_PLAIN, 4,
            Scalar(0,0,255), 3, 3);
        putText(blankimage, maxmean, Point(50,200),FONT_HERSHEY_PLAIN, 4,
                Scalar(0,0,255), 3, 3);
        printf("%.2f, %.2f, %.f\n",minmeans[0],minmeans[1],minmeans[2]);
        printf("%.2f, %.2f, %.f\n\n",maxmeans[0],maxmeans[1],maxmeans[2]);
        
        images.push_back(savegray);
        images.push_back(threshold_frame);
        images.push_back(blankimage);
        Mat out =concatMats(images, 2, 3, 300, 300);
        imshow( "Gray", out );
        char c = waitKey(1);
        if(c =='q'){
            destroyWindow("Gray");
            waitKey(1);
            break;
        }
   
    }
}
