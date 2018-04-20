// VideoProcessor.cpp : implementation file
//

#include "stdafx.h"

#include "ComConstructor.h"
#include "VideoProcessor.h"
#include "ColorDetector.h"
#include "ColorDetectController.h"

#include "MainFrm.h"
#include "ComConstructorDoc.h"

using namespace std;

void ColorBall(cv::Mat& img, cv::Mat& out, int& Threshold, cv::Vec3i& color )
{	
	//�Է� ���� ����
	if(ColorDetectController::getInstance()->setInputImage(img) == false )
		AfxMessageBox(_T("������ �Է¿���"));

	//�Ӱ谪 ����
	ColorDetectController::getInstance()->setColorDistanceThreshold(Threshold);

	//��ü�� ���� ����
	ColorDetectController::getInstance()->setTargetColor(color[2],color[1],color[0]);

	//���� ó��
	ColorDetectController::getInstance()->process();

	//ó������� ��¿������� �Ѱ��ش�.
	out = ColorDetectController::getInstance()->getLastResult();	

	//������ instance ����
	ColorDetectController::getInstance()->destroy();
}