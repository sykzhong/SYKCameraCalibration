// CameraCalibration01.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "GlobalMethod.h"


#include "CAMCalibrator.h"
#include "PNPSolver.h"
#include "Camera.h"
#include "RTMonitor.h"
//You can choose the following code block to run:
//----TEST
//----CALIBRATION
//----RTMONITOR


#define TEST

#ifdef TEST
INITIALIZE_EASYLOGGINGPP
int main()
{
	CAMCalibrator calibrator;
	//calibrator.getPictures("Pictures\\syk");
	calibrator.getPic2Calibrate(false);
}

#elif defined CALIBRATION
int main() {
	CameraCalibrator myCameraCalibrator;
	myCameraCalibrator.setFilename();
	//myCameraCalibrator.setBorderSize(Size(6, 4));		//sykע��
	myCameraCalibrator.setBorderSize(Size(4, 7));
	myCameraCalibrator.addChessboardPoints();

	//Mat src = imread("chessboard09.jpg", 0);
	//Mat src = imread("chess9.bmp", 0);
	//Mat src = imread("sykchess (2).JPG", 0);

	Mat src = imread("sykchess5.JPG", 0);
	Mat dst;
	myCameraCalibrator.calibrate(src, dst);
	myCameraCalibrator.calibrateresult();
	//namedWindow("Undistorted Image", WINDOW_NORMAL);
	//imshow("Original Image", src);
	//imshow("Undistorted Image", dst);
	//imwrite("chess9_d.bmp", dst);
	//waitKey();
	return 0;
}
#elif defined RTMONITOR
int main()
{
	MindVisionCAM mvcam;
	//mvcam.ExposureTimeMS = 60;
	//mvcam.AnalogGain = 4;
	RTMonitor monitor;
	if (mvcam.Init())
	{
		mvcam.StartCapture();

		namedWindow("CamPos", 0);
		cvSetMouseCallback("CamPos", RTMonitor::onMouseHandle, &monitor);
		/********����Calibration�Ľ����������********/
		//����ڲ���
		double fx = 1196.98;
		double fy = 1194.61;
		double u0 = 634.075;
		double v0 = 504.842;
		//��ͷ�������
		double k1 = -0.475732;
		double k2 = 0.405008;
		double p1 = 0.00196334;
		double p2 = -0.00201087;
		double k3 = -0.337634;

		PNPSolver p4psolver;
		p4psolver.SetCameraMatrix(fx, fy, u0, v0);
		p4psolver.SetDistortionCoefficients(k1, k2, p1, p2, k3);

		//�������������������ӽ�ȥ
		p4psolver.Points3D.push_back(cv::Point3f(0, 0, 0));		//P1��ά����ĵ�λ�Ǻ���
		p4psolver.Points3D.push_back(cv::Point3f(0, 200, 0));	//P2
		p4psolver.Points3D.push_back(cv::Point3f(150, 0, 0));	//P3
		p4psolver.Points3D.push_back(cv::Point3f(150, 200, 0));	//P4
		//p4psolver.Points3D.push_back(cv::Point3f(0, 100, 105));	//P5

		cout << "������Ļ�ϵ��ȷ�������㣬������ĵ�ѡ˳��Ӧ��p4psolver.Points3D�Ĵ洢˳��һ�¡�" << endl;
		while (waitKey(1) != 27)
		{
			if (waitKey(1) == 'r')
			{
				monitor.resetCenters();
			}
			monitor.inputImage(mvcam.Grub());
			monitor.trackingCenters();
			monitor.solvePos(p4psolver);
			monitor.showPaintBoard("CamPos");
		}
		mvcam.Release();
	}
	else
		cout << "�����ʼ��ʧ�ܣ�" << endl;
	return 0;
}


#endif