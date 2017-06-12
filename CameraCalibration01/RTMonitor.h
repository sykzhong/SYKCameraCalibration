#ifndef _RTMONITOR_H_
#define _RTMONITOR_H_
#include "PNPSolver.h"

using namespace cv;
using namespace std;

//����չʾһ��ʵʱ���λ�˹���

class RTMonitor 
{
public:
	RTMonitor();
	~RTMonitor();
	void onMouseHandle(int event, int x, int y, int flag, void *param);
	Point2f tracking(Mat image, Point2f lastcenter);		//����������

private:
	vector<Point2f> lastCenters;		//���ڼ�¼���������������
};


#endif