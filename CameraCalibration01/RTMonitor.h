#ifndef _RTMONITOR_H_
#define _RTMONITOR_H_
#include "PNPSolver.h"

using namespace cv;
using namespace std;

//����չʾһ��ʵʱ���λ�˹���

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

	void trackingCenters();											//׷�ٲ����µ��λ��
	static Point2f tracking(Mat &image, Point2f &lastcenter);		//����������
	
	void solvePos(PNPSolver&);		//���λ��

	void inputImage(Mat&);			//����֡ͼ��
	void showPaintBoard(const string&);
private:
	Mat srcImage;					//�洢����֡ͼ��
	Mat paintBoard;					//�洢����ͼ��
	vector<Point2f> lastCenters;	//���ڼ�¼���������������
};


#endif