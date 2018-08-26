#pragma once
#include<iostream>
#include<math.h>
#include<opencv2/opencv.hpp>
#include<stdio.h>

using namespace cv;
using namespace std;

//类声明定义
class Detection {
public:
	Detection();
	Detection(int H);
	Detection(int X, int S);
	Detection(int X, int S, int E);


	void srcImage_Demo(Mat &src);
	void srcImageDemo(Mat &src_I);
	void EllipseSegmention_Demo(Mat &src_S, Mat &src_seg);
	void EllipseDemo(Mat &src_S, Mat &src_seg, Mat &src_E);

private:

	int threshold_value;
	int threshold_max;



};


