#ifndef _RTMONITOR_H_
#define _RTMONITOR_H_
#include "PNPSolver.h"

using namespace cv;
using namespace std;

//用于展示一个实时相机位姿估计

class RTMonitor 
{
public:
	RTMonitor();
	~RTMonitor();
	void onMouseHandle(int event, int x, int y, int flag, void *param);
	Point2f tracking(Mat image, Point2f lastcenter);		//跟踪特征点

private:
	vector<Point2f> lastCenters;		//用于记录特征点的像素坐标
};


#endif