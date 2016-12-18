//
//  gui.cpp
//  GoGameRecorder
//
//  Created by larry on 12/17/16.
//  Copyright © 2016 larry. All rights reserved.
//

#include "gui.hpp"

void callbackButton(int state, void* userdata){}

int makegui(int ac, char **av){
    
    namedWindow("main1",CV_WINDOW_NORMAL);
    namedWindow("main2",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
    int a = 0;
    
    createButton("test",callbackButton, &a, CV_CHECKBOX,1);
    waitKey(0);
    return 0;
}
