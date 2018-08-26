#include"Detection.h"
Mat src_segImage, drawdst;
Mat mor_seg, bin_seg, dst_seg;

Detection::Detection(int X, int S) {
	threshold_value = 72;
	threshold_max = 255;

};
void Detection::EllipseSegmention_Demo(Mat &src_S, Mat &src_seg)
{
	src_segImage = src_seg;
	RNG rng(12345);
	Mat src_copy = src_segImage.clone();

	/*
	1.形态学操作
	2.二值化
	3.寻找最大轮廓
	4.寻找凸包以及凸包缺陷位置等
	*/
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	Mat kernel_open = getStructuringElement(MORPH_RECT, Size(13, 13));
	morphologyEx(src_copy, mor_seg, MORPH_OPEN, kernel_open);
	//imshow("bin_seg2", mor_seg);

	threshold(mor_seg, bin_seg, threshold_value, 255, THRESH_BINARY);
	findContours(bin_seg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> >hull(contours.size());
	vector<vector<int>> hullsI(contours.size());
	vector<vector<Vec4i>> defects(contours.size());

	for (size_t i = 0; i < contours.size(); i++)
	{
		/*
		1.画出灰度距
		2.计算出凸缺陷的起始点、终点、和深度等
		*/
		convexHull(Mat(contours[i]), hull[i], false);
		convexHull(Mat(contours[i]), hullsI[i], false);
		convexityDefects(Mat(contours[i]), hullsI[i], defects[i]);
	}
	/*
	1.掩模矩阵操作
	2.将原始图像克隆在掩模图像上
	*/
	Mat dst_seg = Mat::zeros(bin_seg.size(), CV_8UC3);
	dst_seg = src_segImage.clone();

	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(dst_seg, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		drawContours(dst_seg, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		size_t count = contours[i].size();
		std::cout << "Count : " << count << std::endl;
		if (count < 10)
			continue;
		vector<Vec4i>::iterator d = defects[i].begin();
		while (d != defects[i].end()) {
			Vec4i& v = (*d);
			{
				int startidx = v[0];
				Point ptStart(contours[i][startidx]); // 凸包缺陷的起始点
				int endidx = v[1];
				Point ptEnd(contours[i][endidx]); // 凸包缺陷的终点
				int faridx = v[2];
				Point ptFar(contours[i][faridx]);// 位置
				int depth = v[3] / 256; // 最远距离
				if (depth > 5 && depth < 20)
				{
					line(dst_seg, ptStart, ptFar, CV_RGB(0, 255, 0), 2);
					line(dst_seg, ptEnd, ptFar, CV_RGB(0, 255, 0), 2);
					circle(dst_seg, ptStart, 4, Scalar(255, 0, 100), 2);
					circle(dst_seg, ptEnd, 4, Scalar(255, 0, 100), 2);
					circle(dst_seg, ptFar, 4, Scalar(100, 0, 255), 2);


				}
			}
			d++;
		}
	}
	///*
	//求取最小外接圆和矩形
	//*/
	//cout << "num=" << contours.size() << endl;
	//vector<vector<Point>> contours_ploy(contours.size());
	//vector<Rect> boundRect(contours.size());  //定义外接矩形集合
	//vector<RotatedRect> box(contours.size()); //定义最小外接矩形集合
	//vector<RotatedRect> minRects(contours.size());
	//vector<RotatedRect> myellipse(contours.size());
	//vector<Rect> ploy_rects(contours.size());
	//vector<Point2f> ccs(contours.size());
	//vector<float> radius(contours.size());
	//Point2f rect[4];

	//for (size_t i = 0; i < contours.size(); i++) {
	//	approxPolyDP(Mat(contours[i]), contours_ploy[i], 3, true);
	//	ploy_rects[i] = boundingRect(contours_ploy[i]);
	//	minEnclosingCircle(contours_ploy[i], ccs[i], radius[i]);
	//	if (contours_ploy[i].size() > 5) {
	//		myellipse[i] = fitEllipse(contours_ploy[i]);
	//		minRects[i] = minAreaRect(contours_ploy[i]);
	//	}
	//}
	//for (int i = 0; i < contours.size(); i++) {
	//	box[i] = minAreaRect(Mat(contours[i]));  //计算每个轮廓最小外接矩形
	//	boundRect[i] = boundingRect(Mat(contours[i]));
	//	circle(dst_seg, Point(box[i].center.x, box[i].center.y), 5, Scalar(255, 0, 0), 1, 8);  //绘制最小外接矩形的中心点
	//	box[i].points(rect);  //把最小外接矩形四个端点复制给rect数组
	//	rectangle(dst_seg, Point(boundRect[i].x, boundRect[i].y),
	//		Point(boundRect[i].x + boundRect[i].width,
	//			boundRect[i].y + boundRect[i].height), Scalar(255, 0, 0), 2, 8);
	//	for (int j = 0; j < 4; j++) {
	//		line(dst_seg, rect[j], rect[(j + 1) % 4], Scalar(255, 0, 0), 2, 8);  //绘制最小外接矩形每条边		
	//	}
	//}
	//for (size_t t = 0; t < contours.size(); t++) {
	//	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	//rectangle(drawImg, ploy_rects[t], color, 2, 8);
	//	//circle(drawImg, ccs[t], radius[t], color, 2, 8);
	//	if (contours_ploy[t].size() > 5) {
	//		ellipse(dst_seg, myellipse[t], color, 2, 8);
	//		minRects[t].points(rect);
	//		for (int r = 0; r < 4; r++) {
	//			line(dst_seg, rect[r], rect[(r + 1) % 4], color, 2, 8);
	//		}
	//	}
	//}
	resizeWindow("dst_seg", 500, 500);
	imshow("dst_seg", dst_seg);

}




