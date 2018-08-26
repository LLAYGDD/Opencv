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

	//��ɫ��ȡ
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

	//��ȡ ��ͨ����
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
	vector<Rect> boundRect(contours.size());  //������Ӿ��μ���
	vector<RotatedRect> box(contours.size()); //������С��Ӿ��μ���
	Point2f rect[4];
	for (int i = 0; i < contours.size(); i++) {
		box[i] = minAreaRect(Mat(contours[i]));  //����ÿ��������С��Ӿ���
		boundRect[i] = boundingRect(Mat(contours[i]));
		circle(dstImage, Point(box[i].center.x, box[i].center.y), 5, Scalar(255, 0, 0), 1, 8);  //������С��Ӿ��ε����ĵ�
		box[i].points(rect);  //����С��Ӿ����ĸ��˵㸴�Ƹ�rect����
		rectangle(dstImage, Point(boundRect[i].x, boundRect[i].y),
			Point(boundRect[i].x + boundRect[i].width,
				boundRect[i].y + boundRect[i].height), Scalar(255, 0, 0), 2, 8);
		for (int j = 0; j < 4; j++) {
			line(dstImage, rect[j], rect[(j + 1) % 4], Scalar(255, 0, 0), 2, 8);  //������С��Ӿ���ÿ����		
		}
	}
	imshow("binImage", binImage);
	imshow("dstImage", dstImage);


}








