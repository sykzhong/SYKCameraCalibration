#include "stdafx.h"
#include "Camera.h"

MindVisionCAM::MindVisionCAM()
{

}

MindVisionCAM::~MindVisionCAM()
{
	Release();
}

bool MindVisionCAM::Init()
{
	tSdkCameraDevInfo sCameraList[10];
	INT iCameraNums = 10;	//调用CameraEnumerateDevice前，先设置iCameraNums＝ 10，如果需要枚举更多的设备，请更改sCameraList的值
	CameraSdkStatus status;

	if (CameraEnumerateDevice(sCameraList, &iCameraNums) != CAMERA_STATUS_SUCCESS || iCameraNums == 0)
	{
		printf("No camera was found！");
		return FALSE;
	}
	//该示例中，我们只假设连接了一个相机。因此，只初始化第一个相机。(-1,-1)表示加载上次退出前保存的参数，如果是第一次使用该相机，则加载默认参数.
	//In this demo ,we just init the first camera.
	if ((status = CameraInit(&sCameraList[0], -1, -1, &m_hCamera)) != CAMERA_STATUS_SUCCESS)
	{
		char msg[128];
		sprintf_s(msg, "Failed to init the camera! Error code is %d", status);
		printf(msg);
		return FALSE;
	}

	//Get properties description for this camera.
	tSdkCameraCapbility sCameraInfo;
	CameraGetCapability(m_hCamera, &sCameraInfo);// 获得相机的属性

	//CameraSetAeState(m_hCamera, FALSE);//设置相机曝光的模式。自动或者手动。bState：TRUE，使能自动曝光；FALSE，停止自动曝光。
	//CameraSetExposureTime(m_hCamera, 1000 * ExposureTimeMS);//曝光时间10ms = 1000微秒*10
	//CameraSetAnalogGain(m_hCamera, 10 * AnalogGain);//设置模拟增益16=1.6  该值乘以 CameraGetCapability  获得的相机属性结构体中sExposeDesc.fAnalogGainStep ，就得到实际的图像信号放大倍数。
	//if (ColorType == CV_8U)
	//{
	//	CameraSetMonochrome(m_hCamera, TRUE);//设置黑白图像
	//}
	//CameraSetFrameSpeed(m_hCamera, sCameraInfo.iFrameSpeedDesc - 1);//设定相机输出图像的帧率。iFrameSpeedSel：选择的帧率模式索引号，范围从 0 到CameraGetCapability 获得的信息结构体中	iFrameSpeedDesc - 1

	//if (sCameraInfo.sIspCapacity.bMonoSensor)//ISP 能力描述，BOOL bMonoSensor; //表示该型号相机是否为黑白相机,如果是黑白相机，则颜色相关的功能都无法调节
	//{
	//	CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8);
	//}

	//获取相机配置文件毕竟加载器参数
	
	static const int MAX_LEN = 200;
	TCHAR c_wPath[MAX_LEN];
	GetCurrentDirectory(MAX_LEN, c_wPath);

	wstring wstrPath(&c_wPath[0]);						//convert to wstring
	string strPath(wstrPath.begin(), wstrPath.end());	//and convert to string.
	strPath += "//CAMConfig.Config";

	char *cstr = new char[strPath.length() + 1];
	strcpy(cstr, strPath.c_str());						//convert to char*
	int loadstatus = CameraReadParameterFromFile(m_hCamera, cstr);		

	//if (loadstatus != CAMERA_STATUS_SUCCESS)
	//	QMessageBox::information(NULL, tr("Select file path"), tr("You didn't select any files."));

	//创建该相机的属性配置窗口。
	CameraCreateSettingPage(m_hCamera, NULL, "cam", NULL, NULL, 0);//"通知SDK内部建该相机的属性页面";

	//CameraSetMirror(m_hCamera, 1, TRUE);

	//相机初始化完成
	CameraShowSettingPage(m_hCamera, TRUE);//TRUE显示相机配置界面。FALSE则隐藏

	HasInited = true;
	return true;
}

void MindVisionCAM::GetFrame(cv::Mat & img)
{
	tSdkFrameHead 	sFrameInfo;
	CameraHandle    hCamera = (CameraHandle)m_hCamera;
	BYTE*			pbyBuffer;
	CameraSdkStatus status;

	if (CameraGetImageBuffer(hCamera, &sFrameInfo, &pbyBuffer, 1000) == CAMERA_STATUS_SUCCESS)
	{
		//将获得的原始数据转换成RGB格式的数据，同时经过ISP模块，对图像进行降噪，边沿提升，颜色校正等处理。
		//我公司大部分型号的相机，原始数据都是Bayer格式的
		status = CameraImageProcess(hCamera, pbyBuffer, m_pFrameBuffer, &sFrameInfo);//连续模式
		if (status == CAMERA_STATUS_SUCCESS)
		{
			cv::Mat OriginalImage;
			if (ColorType == CV_8U)
				OriginalImage = cv::Mat(sFrameInfo.iHeight, sFrameInfo.iWidth, CV_8U, m_pFrameBuffer);
			else
				OriginalImage = cv::Mat(sFrameInfo.iHeight, sFrameInfo.iWidth, CV_8UC3, m_pFrameBuffer);
			//去白边
			img = OriginalImage(cv::Rect(2, 2, sFrameInfo.iWidth - 4, sFrameInfo.iHeight - 4)).clone();
		}
		CameraReleaseImageBuffer(hCamera, pbyBuffer);
	}
}

