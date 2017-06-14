#include "stdafx.h"
#include "RTMonitor.h"
#include "GlobalMethod.h"


RTMonitor::RTMonitor()
{

}

RTMonitor::~RTMonitor()
{

}


void RTMonitor::resetCenters()
{
	lastCenters.clear();
	cout << "清除所有特征点，请重新在屏幕上点击确定特征点。" << endl;
}

void RTMonitor::inputImage(Mat& img)
{
	//srcImage = Mat();
	srcImage = img.clone();
	paintBoard = Mat::zeros(srcImage.size(), CV_8UC3);
}

void RTMonitor::onMouseHandle(int event, int x, int y, int flag, void *param)
{
	RTMonitor& monitor = *(RTMonitor*)param;
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		//if (monitor.lastCenters.size() == 4)
		//{
		//	cout << "lastCenters.size has reach 4! do not choose again!" << endl;
		//	return;
		//}
		monitor.lastCenters.push_back(Point2f(x, y));
		cout << "add 1 point:" << monitor.lastCenters.size() << "/4" << endl;
		return;
	}
}

void RTMonitor::trackingCenters()
{
	//if (lastCenters.size() < 4)
	//	cerr << "lastCenters.size() < 4" << endl;
	for (int i = 0; i < lastCenters.size(); i++)
	{
		lastCenters[i] = tracking(srcImage, lastCenters[i]);
		circle(paintBoard, lastCenters[i], 8, RED, 3);
	}
}

Point2f RTMonitor::tracking(Mat& image, Point2f& lastcenter)
{
	const int r = 100;			//检测半径
	const int r2 = r * 2;

	int startx = lastcenter.x - r;
	int starty = lastcenter.y - r;
	if (startx < 0)
		startx = 0;
	if (starty < 0)
		starty = 0;

	if (startx + r2 >= image.size().width)
		startx = image.size().width - 1 - r2;
	if (starty + r2 >= image.size().height)
		starty = image.size().height - 1 - r2;

	Mat roi = image(Rect(startx, starty, r2, r2));
	Mat roiHSV;
	cvtColor(roi, roiHSV, CV_BGR2HSV);

	vector<Mat> hsv;
	split(roiHSV, hsv);
	Mat h = hsv[0];
	Mat s = hsv[1];
	Mat v = hsv[2];
	Mat roiBinary = Mat::zeros(roi.size(), CV_8U);		//二值图像，255表示判断为红色

	/*************判断颜色****************/
	const double ts = 0.5 * 255;//s阈值，小于该值不判断
	const double tv = 0.1 * 255;//v阈值，小于该值不判断
	const double th = 0 * 180 / 360;//h中心
	const double thadd = 30 * 180 / 360;//h范围在th±thadd内的才被算作是红色

	int roiRows = roi.rows;
	int roiCols = roi.cols;
	int roiChannels = roi.channels();
	if (roi.isContinuous() == true)
	{
		roiCols = roiCols*roiRows;
		roiRows = 1;
	}
	for (int i = 0; i < roiRows; i++)
	{
		uchar *ptrh = h.ptr<uchar>(i);
		uchar *ptrs = s.ptr<uchar>(i);
		uchar *ptrv = v.ptr<uchar>(i);
		uchar *ptrbin = roiBinary.ptr<uchar>(i);

		for (int j = 0; j < roiCols; j++)
		{
			if (ptrs[j] < ts || ptrv[j] < tv)
				continue;
			if (th + thadd > 180)
				if (ptrh[j] < th - thadd && ptrh[j] > th + thadd - 180)
					continue;
			if (th - thadd < 0)
				if (ptrh[j] < th - thadd + 180 && ptrh[j] > th + thadd)
					continue;
			ptrbin[j] = 255;		//找出红色像素点，标记为255
		}
	}

	/*****************对二值化图像求出连通域 重心****************/
	vector<vector<Point>> contours;
	cv::findContours(roiBinary.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//分别求出连通域的重心
	vector<Point> gravityCenters;		//重心点集
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() < 10)		//连通域太小
			continue;	
		int xsum = 0;
		int ysum = 0;
		for (int j = 0; j < contours[i].size(); j++)
		{
			xsum += contours[i][j].x;
			ysum += contours[i][j].y;
		}
		double xavr = xsum / contours[i].size();
		double yavr = ysum / contours[i].size();
		gravityCenters.push_back(Point2f(xavr + startx, yavr + starty));
	}

	/*********************返回最优点******************/
	//找到重心跟上一轮位置最接近的那个
	Point ret = lastcenter;
	double dist = 1000000000;
	double distX = 1000000000;
	double distY = 1000000000;
	for (int i = 0; i < gravityCenters.size(); i++)
	{
		if (distX > abs(gravityCenters[i].x - lastcenter.x) && distY > abs(gravityCenters[i].y - lastcenter.y))
		{
			double newdist = sqrt((lastcenter.x - gravityCenters[i].x)*(lastcenter.x - gravityCenters[i].x) + (lastcenter.y - gravityCenters[i].y)*(lastcenter.y - gravityCenters[i].y));
			if (dist > newdist)
			{
				distX = abs(lastcenter.x - gravityCenters[i].x);
				distY = abs(lastcenter.y - gravityCenters[i].y);
				dist = newdist;
				ret = gravityCenters[i];
			}
		}
	}
	return ret;
}

void RTMonitor::solvePos(PNPSolver& p4psolver)
{
	if (lastCenters.size() < 4)
		return;
	//首先将位姿估计类内的特征点的像素坐标记录清0。
	p4psolver.Points2D.clear();
	//然后将新跟踪到的特征点像素坐标插入
	for (int i = 0; i < lastCenters.size(); i++)
	{
		p4psolver.Points2D.push_back(lastCenters[i]);
	}
	//解位姿
	p4psolver.Solve(PNPSolver::METHOD::CV_P3P);

	//将点重投影回图像，检验投影点是否正确
	vector<cv::Point3f> r;
	r.push_back(cv::Point3f(0, 100, 105));//重投影点世界坐标
	vector<cv::Point2f>	ps = p4psolver.WorldFrame2ImageFrame(r);

	//重绘投影点，检验正误
	for (int i = 0; i < ps.size(); i++)
	{
		cv::circle(paintBoard, ps[i], 5, GREEN, -1);
	}
	//输出位姿信息到txt
	ofstream fout1("..\\pnp_theta.txt");
	fout1 << p4psolver.Theta_W2C.x << endl << p4psolver.Theta_W2C.y << endl << p4psolver.Theta_W2C.z << endl;
	fout1.close();
	ofstream fout2("..\\pnp_t.txt");
	fout2 << p4psolver.Position_OcInW.x << endl << p4psolver.Position_OcInW.y << endl << p4psolver.Position_OcInW.z << endl;
	fout2.close();
}

void RTMonitor::showPaintBoard(const string& winname)
{
	imshow(winname, (srcImage - paintBoard) + paintBoard);
}