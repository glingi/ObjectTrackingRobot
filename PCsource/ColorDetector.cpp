#include "StdAfx.h"
#include "ColorDetector.h"
#include "MainFrm.h"
#include <vector>

using namespace cv;

// transfer information of object to AVR 
void ColorDetector::Transfer(cv::Point opt, int size)
{
	// 메인프레임의 포인터를 얻어온다.
	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	
	// Check serial commuication port
	if (pMainFrame->m_Serial.IsOpen())
	{	
		//char ch = static_cast<char>('A');
		//가능
		//pMainFrame->m_Serial.Write("Hello",5);
		//pMainFrame->m_Serial.Write(&pointMark,5);
		//데이터 패킷의 시작~
		pMainFrame->m_Serial.Write("B",1);
		
		_itoa_s(opt.x,temp,10);
		pMainFrame->m_Serial.Write(&temp,strlen(temp));

		pMainFrame->m_Serial.Write("X",1);
		_itoa_s(opt.y,temp,10);
		pMainFrame->m_Serial.Write(&temp,strlen(temp));

		pMainFrame->m_Serial.Write("Y",1);
		_itoa_s(size, temp , 10);
		pMainFrame->m_Serial.Write(&temp,strlen(temp));

		pMainFrame->m_Serial.Write("S",1);
		pMainFrame->m_Serial.Write("E",1);

	}
}

cv::Mat ColorDetector::process(const cv::Mat &image){

//////////////////////////////////////////////////////////////////////////////////////////////////////
//					여러 효과가 적용된 이미지를 저장할 공간
//////////////////////////////////////////////////////////////////////////////////////////////////////

	result.create(image.rows, image.cols, CV_8UC3);
	binary.create(image.rows, image.cols, CV_8U);
	
	// 상태바에 추적점과 contour사이지를 나태내어 주기 위해 메인프레임의 포인터를 얻어온다.
	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
		
//////////////////////////////////////////////////////////////////////////////////////////////////////
//						filtering section
//////////////////////////////////////////////////////////////////////////////////////////////////////

	// remove noise using median filter
	cv::medianBlur(binary, binary ,9);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//						binarization section
//////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//get the iterators
	//obtain iterator at initial position
	cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
	//obtain end position
	cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();
	
	cv::Mat_<uchar>::iterator itout_1C = binary.begin<uchar>();
	cv::Mat_<cv::Vec3b>::iterator itout_3C = result.begin<cv::Vec3b>();

	//for each pixel
	for( ; it!= itend; ++it, ++itout_1C,++itout_3C){

		//process each pixel -----------------

		//comput distance from target color
		if(getDistance(*it)<minDist){

			*itout_1C = 255;
			*itout_3C = *it;

		} else{

			*itout_1C = 0;
			*itout_3C = cv::Vec3b(0,0,0);

		}
		//end of pixel processing --------------------
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//				Get the contours of the connected components
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(binary, 
		contours, // a vector of contours 
		CV_RETR_EXTERNAL, // retrieve the external contours
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours
	

	// Eliminate too short or too long contours
	int cmin= 100;  // minimum contour length
	int cmax= 800; // maximum contour length
	std::vector<std::vector<cv::Point>>::const_iterator itc= contours.begin();
	while (itc!=contours.end()) {

	
		if (itc->size() < cmin || itc->size() > cmax)
			itc= contours.erase(itc);
		else {
			// Draw box around the object 
			object_rect = cv::boundingRect(cv::Mat(*itc)); 
			// cacluate the size of box
			object_size = (object_rect.height * object_rect.width)/50;
			cv::rectangle(result,object_rect,cv::Scalar(255,255,0),2);

			// variables to display status bar 
	
			// compute all moments
			cv::Moments mom= cv::moments(cv::Mat(*itc));
		
			// draw center point 
			cv::Point pt(mom.m10/mom.m00 , mom.m01/mom.m00); 
			// draw mass center
			cv::circle(result,
			// position of mass center converted to integer
						pt,
						2,cv::Scalar(0,255,255),2); // draw dot
		
	
			// cetner position of the object in status bar
			if(pt.x >=0 && pt.y >=0 )
			{
				strText.Format(_T("( %d, %d)"), pt.x, pt.y);
				pMainFrame->m_wndStatusBar.SetPaneText(0,strText);
			}
		
			// size of the object in status bar
			if(itc->size() ){
				strText2.Format(_T(" %d"), object_size );
				pMainFrame->m_wndStatusBar.SetPaneText(1,strText2);
			}
		
			// Transfer x, y coordinate and size of the object to AVR
			ColorDetector::Transfer(pt , object_size);
	
			++itc;
		}
	}
	
	return result;
}
