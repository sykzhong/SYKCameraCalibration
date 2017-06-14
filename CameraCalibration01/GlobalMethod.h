#pragma once
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/highgui/highgui.hpp"  


using namespace std;

static const cv::Scalar RED = cv::Scalar(0, 0, 255);
static const cv::Scalar PINK = cv::Scalar(230, 130, 255);
static const cv::Scalar BLUE = cv::Scalar(255, 0, 0);
static const cv::Scalar LIGHTBLUE = cv::Scalar(255, 255, 160);
static const cv::Scalar GREEN = cv::Scalar(0, 255, 0);
static const cv::Scalar YELLOW = cv::Scalar(0, 255, 255);
static const cv::Scalar ORCHID = cv::Scalar(214, 112, 218);

static const cv::Scalar BLACK = cv::Scalar(0, 0, 0);
static const cv::Scalar WHITE = cv::Scalar(255, 255, 255);

static const double PI = 3.141592654;
class GlobalMethod
{
public:
	static void GetAllFiles(string path, vector<string>& files);
	static void GetAllFormatFiles(string path, vector<string>& files, string format);
	static void GetCurrentFile(string &filepath);
};

