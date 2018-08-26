#include"Detection.h"


//Mat srcImage, bin_Image;
Mat srcEimage, dst_EImage;

Detection::Detection(int X, int S, int E) {
	threshold_value = 50;
	threshold_max = 255;

};

void Detection::EllipseDemo(Mat &src_S, Mat &src_seg, Mat &src_E)
{
	srcEimage = src_E;
	Mat  binary;

	threshold(srcEimage, binary, threshold_value, threshold_max, THRESH_BINARY);
	imshow("binary", binary);

	// 形态学操作
	Mat morphImage;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
	morphologyEx(binary, morphImage, MORPH_OPEN, kernel, Point(-1, -1), 2);
	imshow("morphology", morphImage);

	// 获取最大轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	findContours(morphImage, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	Mat connImage = Mat::zeros(srcEimage.size(), CV_8UC3);
	connImage = srcEimage.clone();
	for (size_t t = 0; t < contours.size(); t++) {
		Rect rect = boundingRect(contours[t]);
		//Mat roi = srcEimage(rect);
		//roi.setTo(255);
		if (rect.width < srcEimage.cols / 2) continue;
		if (rect.width > (srcEimage.cols - 20)) continue;
		double area = contourArea(contours[t]);
		double len = arcLength(contours[t], true);
		drawContours(connImage, contours, static_cast<int>(t), Scalar(0, 255, 255), 4, 8, hireachy);
		printf("area  of star could : %f\n", area);
		printf("length  of star could : %f\n", len);
		//Rect roi = Rect(connImage);
		//roiImage = src(roi);
		//imshow("roi", roi);

	}

	imshow("result", connImage);


}










