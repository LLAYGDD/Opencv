#include"Detection.h"


//Mat srcImage, bin_Image;
Mat src_Image, binImage, dstImage;

Detection::Detection(int H) {
	threshold_value = 72;
	threshold_max = 255;

};
void Detection::srcImageDemo(Mat &src_I)
{
	src_Image = src_I;

	threshold(src_Image, binImage, threshold_value, threshold_max, THRESH_BINARY);

	//反色求取
	Mat newBImg(binImage.rows, binImage.cols, binImage.type());
	uchar* newBImgData = newBImg.data;
	uchar* binaryData = binImage.data;
	int step = binImage.step / sizeof(uchar);
	for (int i = 0; i < binImage.rows; i++)
	{
		for (int j = 0; j < binImage.cols; j++)
		{
			newBImgData[i*step + j] = 255 - binaryData[i*step + j];
		}
	}
	binImage = newBImg.clone();

	//求取 联通区域
	vector<vector<Point>>contours;
	vector<Vec4i> hierachy;
	findContours(binImage, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	dstImage = Mat::zeros(src_Image.size(), CV_8UC3);
	//RNG rng(12345);
	dstImage = src_Image.clone();
	for (size_t i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(255, 0, 0);
		drawContours(dstImage, contours, i, color, 2, 8, hierachy, 0, Point(0, 0));
	}
	cout << "num=" << contours.size() << endl;
	vector<Rect> boundRect(contours.size());  //定义外接矩形集合
	vector<RotatedRect> box(contours.size()); //定义最小外接矩形集合
	Point2f rect[4];
	for (int i = 0; i < contours.size(); i++) {
		box[i] = minAreaRect(Mat(contours[i]));  //计算每个轮廓最小外接矩形
		boundRect[i] = boundingRect(Mat(contours[i]));
		circle(dstImage, Point(box[i].center.x, box[i].center.y), 5, Scalar(255, 0, 0), 1, 8);  //绘制最小外接矩形的中心点
		box[i].points(rect);  //把最小外接矩形四个端点复制给rect数组
		rectangle(dstImage, Point(boundRect[i].x, boundRect[i].y),
			Point(boundRect[i].x + boundRect[i].width,
				boundRect[i].y + boundRect[i].height), Scalar(255, 0, 0), 2, 8);
		for (int j = 0; j < 4; j++) {
			line(dstImage, rect[j], rect[(j + 1) % 4], Scalar(255, 0, 0), 2, 8);  //绘制最小外接矩形每条边		
		}
	}
	imshow("binImage", binImage);
	imshow("dstImage", dstImage);


}








