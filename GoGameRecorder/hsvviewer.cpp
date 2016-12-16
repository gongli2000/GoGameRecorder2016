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
        
        Mat out;
        hconcat(color_img,cframe,out);
        resize(out,out,Size(1000,600));
        cv::imshow(window_name, out);
        
        
        int c = cv::waitKey(10);
        if (c == 'q') break;
    }

}
