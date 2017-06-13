#pragma once
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "Camera.h"

//using namespace cv;
using namespace std;

class CAMCalibrator {
private:
	vector<string> m_filenames;
	cv::Size m_borderSize;
	vector<vector<cv::Point2f> > m_srcPoints;
	vector<vector<cv::Point3f> > m_dstPoints;
public:
	void setFilename();
	void setBorderSize(const cv::Size &borderSize);
	void addChessboardPoints();
	void addPoints(const vector<cv::Point2f> &imageCorners, const vector<cv::Point3f> &objectCorners);
	void calibrate(const cv::Mat &src, cv::Mat &dst);
	void calErr();
	void calibrateresult();
	void getPictures(string Prefix = "syk");
private:
	vector<cv::Mat> rvecs, tvecs;
	cv::Mat cameraMatrix, distCoeffs, map1, map2;
};