#pragma once
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "Camera.h"
#include "easylogging++.h"

//using namespace cv;
using namespace std;
using namespace el;



class CAMCalibrator {

public:
	CAMCalibrator();
	~CAMCalibrator();

	void setFilename();									//设置文件名容器
	void setBorderSize(const cv::Size &borderSize);		//设置棋盘大小
	void addChessboardPoints();							//提取棋盘角点并存储（二维及三维）
	//void addPoints(const vector<cv::Point2f> &imageCorners, const vector<cv::Point3f> &objectCorners);		//输入棋盘角点的二维三维坐标
	void calibrate();									//进行校正提取
	void calErr();										//判断校正误差
	void remapPic();									//对拍摄的所有图像进行校正
	void getPictures(string Prefix = "syk");			//拍摄图像

	void getPic2Calibrate(bool automode = false);		//完整的校正流程

private:
	vector<string> m_filenames;						//用于存储进行校正的图像文件名
	cv::Size m_borderSize;							//棋盘横纵交点数量
	vector<vector<cv::Point2f> > m_srcPoints;		//棋盘交点在图像中的坐标
	vector<vector<cv::Point3f> > m_dstPoints;		//棋盘交点的三维空间坐标

	Size imageSize;									//用于校正的图像的尺寸
	vector<cv::Mat> rvecs, tvecs;					//校正所得参数（calibrateCamera）
	cv::Mat cameraMatrix, distCoeffs, map1, map2;	//相机内参、畸变系数，以及remap所用参数

	string Prefix;									//存储图像文件的前缀名
};