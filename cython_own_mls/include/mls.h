#ifndef MLS_H
#define MLS_H
#include <iostream>
#include <cstdio>
#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"

using std::vector;
using cv::Mat;
using cv::Mat_;
using cv::Point_;
using cv::Point;
using cv::Vec3b;

using namespace std;
using namespace cv;

// #ifdef __cplusplus 
// extern "C" { 
// #endif
// extern void mls_deformation(int height, int width, uchar* oriImg,  uchar* dstImg, int *x1, int *y1, int n, int *x2, int *y2);
// #ifdef __cplusplus 

void cmls_deformation(int height, int width, uchar* oriImg,  uchar* dstImg, int *x1, int *y1, int n, int *x2, int *y2);
#endif
