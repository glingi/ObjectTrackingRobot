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
	//입력 영상 설정
	if(ColorDetectController::getInstance()->setInputImage(img) == false )
		AfxMessageBox(_T("프레임 입력오류"));

	//임계값 설정
	ColorDetectController::getInstance()->setColorDistanceThreshold(Threshold);

	//물체의 색을 설정
	ColorDetectController::getInstance()->setTargetColor(color[2],color[1],color[0]);

	//영상 처리
	ColorDetectController::getInstance()->process();

	//처리결과를 출력영상으로 넘겨준다.
	out = ColorDetectController::getInstance()->getLastResult();	

	//생성된 instance 삭제
	ColorDetectController::getInstance()->destroy();
}