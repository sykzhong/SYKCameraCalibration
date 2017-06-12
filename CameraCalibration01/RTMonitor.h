#ifndef _RTMONITOR_H_
#define _RTMONITOR_H_
#include "PNPSolver.h"

using namespace cv;
using namespace std;

//用于展示一个实时相机位姿估计

static const Scalar RED = Scalar(0, 0, 255);
static const Scalar PINK = Scalar(230, 130, 255);
static const Scalar BLUE = Scalar(255, 0, 0);
static const Scalar LIGHTBLUE = Scalar(255, 255, 160);
static const Scalar GREEN = Scalar(0, 255, 0);
static const Scalar YELLOW = Scalar(0, 255, 255);
static const Scalar ORCHID = Scalar(214, 112, 218);

static const Scalar BLACK = Scalar(0, 0, 0);
static const Scalar WHITE = Scalar(255, 255, 255);

static const double PI = 3.141592654;

class RTMonitor 
{
public:
	RTMonitor();
	~RTMonitor();
	void resetCenters();
	static void onMouseHandle(int event, int x, int y, int flag, void *param);

	void trackingCenters();											//追踪并更新点的位置
	static Point2f tracking(Mat &image, Point2f &lastcenter);		//跟踪特征点
	
	void solvePos(PNPSolver&);		//求解位姿

	void inputImage(Mat&);			//输入帧图像
	void showPaintBoard(const string&);
private:
	Mat srcImage;					//存储输入帧图像
	Mat paintBoard;					//存储绘制图像
	vector<Point2f> lastCenters;	//用于记录特征点的像素坐标
};


#endif