//
//  drawing.cpp
//  opencvplayground
//
//  Created by larry on 12/11/16.
//  Copyright © 2016 larry. All rights reserved.
//

#include "drawing.hpp"
#include <list>

void drawgrid(Mat &img, int delta, int boardsize, Scalar color, int thickness )
{
    Point sq_size((img.cols - 2*delta)/(boardsize-1), (img.rows -2 *delta)/(boardsize-1));
    int row =delta,col=delta;
    for(int i = 0; i < boardsize-1; i++){
        line(img,Point(delta,row), Point(img.cols-delta,row),color,thickness);
        line(img,Point(col,delta), Point(col,img.rows-delta), color,thickness);
        row += sq_size.y;
        col += sq_size.x;
    }
    line(img,Point(delta,img.rows-delta), Point(img.cols-delta,img.rows-delta),color,thickness);
    line(img,Point(img.cols-delta,delta), Point(img.cols-delta,img.rows-delta), color,thickness);
}


void drawpoly(Mat& inputImage, vector<Point> &r, Scalar color, int thickness){
    for(int i=0;i<r.size();i++){
        line(inputImage,r[i],r[(i+1) % r.size()],color,thickness);
    }
    
}


Mat concatMats(vector<vector<Mat>> &images,int width, int height)
{
    Mat hout,vout;
    int k = 0;
    for(auto &imagevec : images){
        int i = 0;
        for(auto &image : imagevec){
            resize(image,image,Size(width,height));
            if(i++ == 0){
                hout = image.clone();
            }else{
                hconcat(hout,image,hout);
            }
        }
        if(k++ ==0){
            vout = hout.clone();
        }else{
            vconcat(vout,hout,vout);
        }
    }
    return vout;
}



