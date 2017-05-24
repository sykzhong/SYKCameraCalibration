// CameraCalibration01.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "calibration.h"

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