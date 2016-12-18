//
//  io.cpp
//  GoGameRecorder
//
//  Created by larry on 12/15/16.
//  Copyright © 2016 larry. All rights reserved.
//

#include "io.hpp"


void saveCameraImage(VideoCapture& videocam,string filename){

    Mat cframe;
    videocam >> cframe;
    imwrite(filename,cframe);
}
    
