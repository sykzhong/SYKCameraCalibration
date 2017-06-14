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

	void setFilename();									//�����ļ�������
	void setBorderSize(const cv::Size &borderSize);		//�������̴�С
	void addChessboardPoints();							//��ȡ���̽ǵ㲢�洢����ά����ά��
	//void addPoints(const vector<cv::Point2f> &imageCorners, const vector<cv::Point3f> &objectCorners);		//�������̽ǵ�Ķ�ά��ά����
	void calibrate();									//����У����ȡ
	void calErr();										//�ж�У�����
	void remapPic();									//�����������ͼ�����У��
	void getPictures(string Prefix = "syk");			//����ͼ��

	void getPic2Calibrate(bool automode = false);		//������У������

private:
	vector<string> m_filenames;						//���ڴ洢����У����ͼ���ļ���
	cv::Size m_borderSize;							//���̺��ݽ�������
	vector<vector<cv::Point2f> > m_srcPoints;		//���̽�����ͼ���е�����
	vector<vector<cv::Point3f> > m_dstPoints;		//���̽������ά�ռ�����

	Size imageSize;									//����У����ͼ��ĳߴ�
	vector<cv::Mat> rvecs, tvecs;					//У�����ò�����calibrateCamera��
	cv::Mat cameraMatrix, distCoeffs, map1, map2;	//����ڲΡ�����ϵ�����Լ�remap���ò���

	string Prefix;									//�洢ͼ���ļ���ǰ׺��
};