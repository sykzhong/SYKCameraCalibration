#include "stdafx.h"
#include "CAMCalibrator.h"
using namespace cv;

void CAMCalibrator::setFilename() {
	m_filenames.clear();
	//m_filenames.push_back("chess1.bmp");
	//m_filenames.push_back("chess2.bmp");
	//m_filenames.push_back("chess3.bmp");
	//m_filenames.push_back("chess4.bmp");
	//m_filenames.push_back("chess5.bmp");
	//m_filenames.push_back("chess6.bmp");
	//m_filenames.push_back("chess7.bmp");
	//m_filenames.push_back("chess8.bmp");
	//m_filenames.push_back("chess9.bmp");
	//m_filenames.push_back("chess10.bmp");
	//m_filenames.push_back("chess11.bmp");
	//m_filenames.push_back("chess12.bmp");
	//m_filenames.push_back("chess13.bmp");
	//m_filenames.push_back("chess14.bmp");

	//m_filenames.push_back("chessboard01.jpg");
	//m_filenames.push_back("chessboard02.jpg");
	//m_filenames.push_back("chessboard03.jpg"); 
	//m_filenames.push_back("chessboard04.jpg");
	//m_filenames.push_back("chessboard05.jpg");
	//m_filenames.push_back("chessboard06.jpg"); 
	//m_filenames.push_back("chessboard07.jpg");
	//m_filenames.push_back("chessboard08.jpg");
	//m_filenames.push_back("chessboard09.jpg");
	//m_filenames.push_back("chessboard10.jpg");
	//m_filenames.push_back("chessboard11.jpg");
	//m_filenames.push_back("chessboard12.jpg");
	//m_filenames.push_back("chessboard13.jpg"); 
	//m_filenames.push_back("chessboard14.jpg");

	//m_filenames.push_back("sykchess (1).JPG");
	//m_filenames.push_back("sykchess (2).JPG");
	//m_filenames.push_back("sykchess (3).JPG");
	//m_filenames.push_back("sykchess (4).JPG");
	//m_filenames.push_back("sykchess (5).JPG");
	//m_filenames.push_back("sykchess (6).JPG");
	//m_filenames.push_back("sykchess (7).JPG");
	//m_filenames.push_back("sykchess (8).JPG");
	//m_filenames.push_back("sykchess (9).JPG");
	//m_filenames.push_back("sykchess (10).JPG");
	//m_filenames.push_back("sykchess (11).JPG");

	m_filenames.push_back("sykchess1.JPG");
	m_filenames.push_back("sykchess2.JPG");
	m_filenames.push_back("sykchess3.JPG");
	m_filenames.push_back("sykchess4.JPG");
	m_filenames.push_back("sykchess5.JPG");
	m_filenames.push_back("sykchess6.JPG");
	m_filenames.push_back("sykchess7.JPG");
	m_filenames.push_back("sykchess8.JPG");
	m_filenames.push_back("sykchess9.JPG");
	m_filenames.push_back("sykchess10.JPG");
	m_filenames.push_back("sykchess11.JPG");
	m_filenames.push_back("sykchess12.JPG");
	m_filenames.push_back("sykchess13.JPG");
	m_filenames.push_back("sykchess14.JPG");
	m_filenames.push_back("sykchess15.JPG");
	m_filenames.push_back("sykchess16.JPG");
	m_filenames.push_back("sykchess17.JPG");
	m_filenames.push_back("sykchess18.JPG");
	m_filenames.push_back("sykchess19.JPG");
	m_filenames.push_back("sykchess20.JPG");
	//m_filenames.push_back("sykchess21.JPG");
}


void CAMCalibrator::setBorderSize(const Size &borderSize) 
{
	m_borderSize = borderSize;
}

void CAMCalibrator::addChessboardPoints() 
{
	vector<Point2f> srcCandidateCorners;
	vector<Point3f> dstCandidateCorners;
	for (int i = 0; i<m_borderSize.height; i++) {
		for (int j = 0; j<m_borderSize.width; j++) {
			//dstCandidateCorners.push_back(Point3f(i, j, 0.0f));		
			dstCandidateCorners.push_back(Point3f(i*10, j*10, 0.0f));		
		}
	}

	for (int i = 0; i<m_filenames.size(); i++) {
		Mat image = imread(m_filenames[i], CV_LOAD_IMAGE_GRAYSCALE);
		findChessboardCorners(image, m_borderSize, srcCandidateCorners);
		find4QuadCornerSubpix(image, srcCandidateCorners, Size(5, 5)); //对粗提取的角点进行精确化	syk

		//TermCriteria param(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1);
		//cornerSubPix(image, srcCandidateCorners, Size(5, 5), Size(-1, -1), param);
		drawChessboardCorners(image, m_borderSize, srcCandidateCorners, true); //用于在图片中标记角点
		//imshow("Camera Calibration", image);//显示图片
		//waitKey(500);
		if (srcCandidateCorners.size() == m_borderSize.area()) {
			addPoints(srcCandidateCorners, dstCandidateCorners);
		}
	}
}

void CAMCalibrator::addPoints(const vector<Point2f> &srcCorners, const vector<Point3f> &dstCorners) {
	m_srcPoints.push_back(srcCorners);
	m_dstPoints.push_back(dstCorners);
}

void CAMCalibrator::calibrate(const Mat &src, Mat &dst) 
{
	Size imageSize = src.size();
	calibrateCamera(m_dstPoints, m_srcPoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);
	calErr();
	cout << "cameraMatrix = " << cameraMatrix << endl;
	cout << "distCoeffs = " << distCoeffs << endl;
	Mat newcameraMatrix = Mat(3, 3, CV_32F, Scalar::all(0));
	//initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), imageSize, CV_32F, map1, map2);
	//initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat::eye(3, 3, CV_32F), Mat(), imageSize, CV_32F, map1, map2);		//syk
	initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), 
		getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0), imageSize, CV_32F, map1, map2);

	cout << newcameraMatrix;																	
	remap(src, dst, map1, map2, INTER_LINEAR);
	//remap(src, dst, map1, map2, INTER_MAX);
}

void CAMCalibrator::calibrateresult()
{

	for (int i = 0; i<m_filenames.size(); i++) 
	{
		Mat image = imread(m_filenames[i], CV_LOAD_IMAGE_GRAYSCALE);
		Mat result = Mat();
		remap(image, result, map1, map2, INTER_LINEAR);
		stringstream strnum;
		strnum << i;
		string resultname = strnum.str();
		resultname += m_filenames[i];
		imwrite(resultname, result);
	}
}

void CAMCalibrator::calErr()
{
	double total_err = 0.0; /* 所有图像的平均误差的总和 */
	double err = 0.0; /* 每幅图像的平均误差 */
	vector<Point2f> image_points2; /* 保存重新计算得到的投影点 */
	cout << "\t每幅图像的标定误差：\n";
	for (int i = 0;i < m_filenames.size();i++)
	{
		vector<Point3f> tempPointSet = m_dstPoints[i];
		/* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
		projectPoints(tempPointSet, rvecs[i], tvecs[i], cameraMatrix, distCoeffs, image_points2);
		/* 计算新的投影点和旧的投影点之间的误差*/
		vector<Point2f> tempImagePoint = m_srcPoints[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (int j = 0; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		total_err += err /= m_borderSize.width*m_borderSize.height;
		std::cout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
	}
	std::cout << "总体平均误差：" << total_err / m_filenames.size() << "像素" << endl;
	std::cout << "评价完成！" << endl;
	//保存定标结果  	
}

void CAMCalibrator::getPictures(string Prefix)
{
	MindVisionCAM mvcam;

	if (mvcam.Init())
	{
		mvcam.StartCapture();
		cout << "Press 'c' to capture picture, Press esc to exit" << endl;

		string winName = "Picture";
		namedWindow(winName);
		Mat m_picture;

		int picCount = 0;
		while(waitKey(1) != 27)
		{
			m_picture = mvcam.Grub();
			imshow(winName, m_picture);
			if (waitKey(1) == 'c')
			{
				char c_num[20];
				sprintf(c_num, "%03d", picCount);
				string filename = Prefix + c_num + ".jpg";
				imwrite(filename, m_picture);
				cout << "Store picture " << filename << endl;
				picCount++;
			}
		}
	}
}