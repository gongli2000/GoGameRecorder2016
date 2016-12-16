//
//  hsvviewer.cpp
//  GoGameRecorder
//
//  Created by larry on 12/12/16.
//  Copyright © 2016 larry. All rights reserved.
//


#include "hsvviewer.hpp"

int h_ = 255;   // H : 0～179, Hue, 色相 (0:赤, 60: 緑, 120:青)
int s_ = 255; // S : 0～255, Saturation, 彩度 (0に近づくほど白くなる)
int v_ = 255; // V : 0～255, Brightness Value, 明度 (0に近づくほど黒くなる)

//void on_trackbar( int x, void* v )
//{
//    printf(  "H %d\n", h_);
//    
//}
void hsvviewer()
{
    Mat color_img, color_hsv;
    
    const char *window_name = "HSV color";
    cv::namedWindow(window_name);
    
    createTrackbar("H", window_name, &h_, 255);//, on_trackbar);
    createTrackbar("S", window_name, &s_, 255);//, on_trackbar);
    createTrackbar("V", window_name, &v_, 255);//, on_trackbar);
    VideoCapture videocam(0);
    if(!videocam.isOpened())return;
    
    while(true) {
        Mat cframe;
        videocam >> cframe;
        cvtColor(cframe,cframe,CV_RGB2HSV);
        color_hsv =cframe.clone();
        color_hsv.setTo(Scalar(h_,s_,v_));
        cv::cvtColor(color_hsv, color_img, CV_HSV2BGR);

        vector<Mat> images={color_img,color_img,cframe,cframe,cframe};
        Mat out= concatMats(images,2,3,300,300);
        cv::imshow(window_name, out);
        
        
        int c = cv::waitKey(10);
        if (c == 'q') break;
    }

}





int viewer2()
{
    VideoCapture cap(0); //capture the video from web cam
    
    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    
    int iLowH = 0;
    int iHighH = 179;
    
    int iLowS = 0;
    int iHighS = 255;
    
    int iLowV = 0;
    int iHighV = 255;
    
    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);
    
    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);
    
    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);
    
    while (true)
    {
        Mat imgOriginal;
        
        bool bSuccess = cap.read(imgOriginal); // read a new frame from video
        
        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        
        Mat imgHSV;
        
        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
        
        Mat imgThresholded;
        
        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
        
        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
        imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original", imgOriginal); //show the original image
        
        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }
    
    return 0;
    
}
