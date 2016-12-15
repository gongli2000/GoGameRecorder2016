//
//  io.cpp
//  GoGameRecorder
//
//  Created by larry on 12/15/16.
//  Copyright © 2016 larry. All rights reserved.
//

#include "io.hpp"


void saveCameraImage(string filename){
    
    VideoCapture videocam(0);
    if(!videocam.isOpened())return;
    
    Mat cframe;
    videocam >> cframe;
    imwrite(filename,cframe);
}
    
