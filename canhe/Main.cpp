#include"Detection.h"

const char*winTitle = "inputImage";


int main() {

	Mat srcImage = imread("image/canhe_1.jpg", IMREAD_GRAYSCALE);
	Mat src_Image = imread("image/canhe_2.jpg", IMREAD_GRAYSCALE);
	Mat src_EImage = imread("image/canhe_3.jpg", IMREAD_GRAYSCALE);
	//边缘缺陷图像
	Mat seg_Image = imread("image/canhe_3.jpg", IMREAD_GRAYSCALE);


	if (srcImage.empty() && src_Image.empty() && seg_Image.empty() && src_EImage.empty()) {
		printf("can not load image..\n");
		return -1;
	}

	resizeWindow(winTitle, 500, 500);
	imshow(winTitle, srcImage);
	imshow("src_Image", src_Image);
	//椭圆形餐盒的初始化创建
	Detection detection;
	detection.srcImage_Demo(srcImage);

	//六边形餐盒的初始化创建
	Detection* detectionA = new Detection(1);
	detectionA->srcImageDemo(src_Image);
	imshow("src_Image", src_Image);

	//分割图像初始化操作
	Detection* detectionSeg = new Detection(1, 2);
	detectionSeg->EllipseSegmention_Demo(src_Image, seg_Image);
	imshow("seg_Image", seg_Image);

	//分割图像初始化操作
	Detection* detectionE = new Detection(1, 1, 1);
	detectionE->EllipseDemo(src_Image, seg_Image, src_EImage);
	imshow("seg_Image", src_EImage);




	waitKey();
	return 0;
}
