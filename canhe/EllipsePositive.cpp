#include"Detection.h"


//Mat srcImage, bin_Image;
Mat srcImage, bin_Image, dst;

Detection::Detection() {
	threshold_value = 72;
	threshold_max = 255;

};

void Detection::srcImage_Demo(Mat &src)
{
	srcImage = src;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7), Point(-1, -1));
	morphologyEx(srcImage, bin_Image, MORPH_OPEN, kernel, Point(-1, -1));
	//Mat kernel_close = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
	//morphologyEx(bin_Image, bin_Image, MORPH_CLOSE, kernel_close, Point(-1, -1));
	threshold(bin_Image, bin_Image, threshold_value, threshold_max, THRESH_BINARY);

	//反色求取
	Mat newBImg(bin_Image.rows, bin_Image.cols, bin_Image.type());
	uchar* newBImgData = newBImg.data;
	uchar* binaryData = bin_Image.data;
	int step = bin_Image.step / sizeof(uchar);
	for (int i = 0; i < bin_Image.rows; i++)
	{
		for (int j = 0; j < bin_Image.cols; j++)
		{
			newBImgData[i*step + j] = 255 - binaryData[i*step + j];
		}
	}
	bin_Image = newBImg.clone();

	//求取 联通区域
	vector<vector<Point>>contours;
	vector<Vec4i> hierachy;
	findContours(bin_Image, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	dst = Mat::zeros(srcImage.size(), CV_8UC3);
	//RNG rng(12345);
	dst = srcImage.clone();
	for (size_t i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(255, 0, 0);
		drawContours(dst, contours, i, color, 2, 8, hierachy, 0, Point(0, 0));
	}
	cout << "num=" << contours.size() << endl;
	vector<Rect> boundRect(contours.size());  //定义外接矩形集合
	vector<RotatedRect> box(contours.size()); //定义最小外接矩形集合
	Point2f rect[4];
	for (int i = 0; i < contours.size(); i++) {
		box[i] = minAreaRect(Mat(contours[i]));  //计算每个轮廓最小外接矩形
		boundRect[i] = boundingRect(Mat(contours[i]));
		circle(dst, Point(box[i].center.x, box[i].center.y), 5, Scalar(255, 0, 0), 1, 8);  //绘制最小外接矩形的中心点
		box[i].points(rect);  //把最小外接矩形四个端点复制给rect数组
		rectangle(dst, Point(boundRect[i].x, boundRect[i].y),
			Point(boundRect[i].x + boundRect[i].width,
				boundRect[i].y + boundRect[i].height), Scalar(255, 0, 0), 2, 8);
		for (int j = 0; j < 4; j++) {
			line(dst, rect[j], rect[(j + 1) % 4], Scalar(255, 0, 0), 2, 8);  //绘制最小外接矩形每条边		
		}
	}
	imshow("bin_Image", bin_Image);
	imshow("dst", dst);
}








