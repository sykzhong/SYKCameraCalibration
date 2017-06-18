// CameraCalibration01.cpp : 定义控制台应用程序的入口点。
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

INITIALIZE_EASYLOGGINGPP

#define RTMONITOR

#ifdef TEST

int main()
{
	CAMCalibrator calibrator;
	//calibrator.getPictures("Pictures\\syk");
	calibrator.getPic2Calibrate(true);
}

#elif defined CALIBRATION
int main() {
	CAMCalibrator myCameraCalibrator;
	myCameraCalibrator.setFilename();
	//myCameraCalibrator.setBorderSize(Size(6, 4));		//syk注释
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
		/********需结合Calibration的结果进行输入********/
		////相机内参数
		//double fx = 1138.762;
		//double fy = 1136.191;
		//double u0 = 528.117;
		//double v0 = 379.210;
		////镜头畸变参数
		//double k1 = -0.1468817;
		//double k2 = 0.383661;
		//double p1 = -0.000137034;
		//double p2 = -0.00293748;
		//double k3 = -1.026599;

		//相机内参数
		double fx = 1202.185;
		double fy = 1202.363;
		double u0 = 535.805;
		double v0 = 371.326;
		//镜头畸变参数
		double k1 = -0.211768;
		double k2 = 0.765674;
		double p1 = 0.00315177;
		double p2 = -0.000973491;
		double k3 = -1.275098;

		PNPSolver p4psolver;
		p4psolver.SetCameraMatrix(fx, fy, u0, v0);
		p4psolver.SetDistortionCoefficients(k1, k2, p1, p2, k3);

		//将特征点的世界坐标添加进去
		p4psolver.Points3D.push_back(cv::Point3f(0, 0, 0));		//P1三维坐标的单位是毫米
		p4psolver.Points3D.push_back(cv::Point3f(0, 200, 0));	//P2
		p4psolver.Points3D.push_back(cv::Point3f(150, 0, 0));	//P3
		p4psolver.Points3D.push_back(cv::Point3f(150, 200, 0));	//P4
		//p4psolver.Points3D.push_back(cv::Point3f(0, 100, 105));	//P5

		cout << "请在屏幕上点击确定特征点，特征点的点选顺序应与p4psolver.Points3D的存储顺序一致。" << endl;
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
		cout << "相机初始化失败！" << endl;
	return 0;
}


#endif