#include"Detection.h"

const char*winTitle = "inputImage";


int main() {

	Mat srcImage = imread("image/canhe_1.jpg", IMREAD_GRAYSCALE);
	Mat src_Image = imread("image/canhe_2.jpg", IMREAD_GRAYSCALE);
	Mat src_EImage = imread("image/canhe_3.jpg", IMREAD_GRAYSCALE);
	//��Եȱ��ͼ��
	Mat seg_Image = imread("image/canhe_3.jpg", IMREAD_GRAYSCALE);


	if (srcImage.empty() && src_Image.empty() && seg_Image.empty() && src_EImage.empty()) {
		printf("can not load image..\n");
		return -1;
	}

	resizeWindow(winTitle, 500, 500);
	imshow(winTitle, srcImage);
	imshow("src_Image", src_Image);
	//��Բ�βͺеĳ�ʼ������
	Detection detection;
	detection.srcImage_Demo(srcImage);

	//�����βͺеĳ�ʼ������
	Detection* detectionA = new Detection(1);
	detectionA->srcImageDemo(src_Image);
	imshow("src_Image", src_Image);

	//�ָ�ͼ���ʼ������
	Detection* detectionSeg = new Detection(1, 2);
	detectionSeg->EllipseSegmention_Demo(src_Image, seg_Image);
	imshow("seg_Image", seg_Image);

	//�ָ�ͼ���ʼ������
	Detection* detectionE = new Detection(1, 1, 1);
	detectionE->EllipseDemo(src_Image, seg_Image, src_EImage);
	imshow("seg_Image", src_EImage);




	waitKey();
	return 0;
}
