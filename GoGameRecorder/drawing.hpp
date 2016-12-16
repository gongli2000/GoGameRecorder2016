//
//  drawing.hpp
//  opencvplayground
//
//  Created by larry on 12/11/16.
//  Copyright © 2016 larry. All rights reserved.
//

#ifndef drawing_hpp
#define drawing_hpp

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
using namespace cv;

Mat concatMats(vector<Mat> &images,int nrows,int ncols,int width, int height);
void drawgrid(Mat &img, int delta, int boardsize, Scalar color, int thickness );

void drawpoly(Mat& inputImage, vector<Point> &r, Scalar color, int thickness);

#endif /* drawing_hpp */
