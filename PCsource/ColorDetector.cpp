#include "StdAfx.h"
#include "ColorDetector.h"
#include "MainFrm.h"
#include <vector>

using namespace cv;

//물체의 정보를 AVR로 전송하기 위한 함수.
void ColorDetector::Transfer(cv::Point opt, int size)
{
	//메인프레임의 포인터를 얻어온다.
	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	
	//직렬통신 포트가 연결되었는지 확인
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
	
	//상태바에 추적점과 contour사이지를 나태내어 주기 위해 메인프레임의 포인터를 얻어온다.
	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
		
//////////////////////////////////////////////////////////////////////////////////////////////////////
//						필터링 부분
//////////////////////////////////////////////////////////////////////////////////////////////////////

	//median필터로 잡음 제거
	cv::medianBlur(binary, binary ,9);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//						이진화 부분
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
			//물체주위에 사각형을 그려줌
			object_rect = cv::boundingRect(cv::Mat(*itc)); 
			//사각형의 크기계산
			object_size = (object_rect.height * object_rect.width)/50;
			cv::rectangle(result,object_rect,cv::Scalar(255,255,0),2);

			//상태바에 정보표시를 하기위한 변수들
	
			// compute all moments
			cv::Moments mom= cv::moments(cv::Mat(*itc));
		
			//중심점 그리기
			cv::Point pt(mom.m10/mom.m00 , mom.m01/mom.m00); 
			// draw mass center
			cv::circle(result,
			// position of mass center converted to integer
						pt,
						2,cv::Scalar(0,255,255),2); // draw dot
		
	
			//상태바에 물체의 중심점 나타내기
			if(pt.x >=0 && pt.y >=0 )
			{
				strText.Format(_T("( %d, %d)"), pt.x, pt.y);
				pMainFrame->m_wndStatusBar.SetPaneText(0,strText);
			}
		
			//상태바에 물체의 사이즈 나타내기
			if(itc->size() ){
				strText2.Format(_T(" %d"), object_size );
				pMainFrame->m_wndStatusBar.SetPaneText(1,strText2);
			}
		
			//중심점의 x,y좌표와 물체의 사이즈를 avr로 전송
			ColorDetector::Transfer(pt , object_size);
	

			++itc;
		}
	}
	
	return result;
}



/*
	// draw contours
	cv::drawContours(result,contours,
		-1,			 // draw all contours
		cv::Scalar(200),		 // 
		2);			 // with a thickness of 2
*/


/*
	// testing the bounding box 
	cv::Rect r0= cv::boundingRect(cv::Mat(contours[0]));
	cv::rectangle(result,r0,cv::Scalar(0),2);

	// testing the enclosing circle 
	float radius;
	cv::Point2f center;
	cv::minEnclosingCircle(cv::Mat(contours[1]),center,radius);
	cv::circle(result,cv::Point(center),static_cast<int>(radius),cv::Scalar(0),2);

//	cv::RotatedRect rrect= cv::fitEllipse(cv::Mat(contours[1]));
//	cv::ellipse(result,rrect,cv::Scalar(0),2);

	// testing the approximate polygon

	std::vector<cv::Point> poly;
	itc = contours.begin();

	for(; itc != contours.end() ; ++itc){
		cv::approxPolyDP(cv::Mat(contours[2]),poly,5,true);

		// Iterate over each segment and draw it
		std::vector<cv::Point>::const_iterator itp= poly.begin();
		while (itp!=(poly.end()-1)) {
			cv::line(out,*itp,*(itp+1),cv::Scalar(0),2);
			++itp;
		}
		// last point linked to first point
		cv::line(out,
				*(poly.begin()),
				*(poly.end()-1),cv::Scalar(20),2);
	}
	*/

//cv::Mat result(image.size(),CV_8UC3,cv::Scalar(5,255,255));

/*
	itc = contours.begin();
	while (itc!=contours.end()) {
		//물체주위에 사각형을 그려줌
		object_rect = cv::boundingRect(cv::Mat(*itc)); 
		//사각형의 크기계산
		object_size = (object_rect.height * object_rect.width)/50;
		cv::rectangle(result,object_rect,cv::Scalar(255,255,0),2);
		++itc;
	}

	*/
	// iterate over all contours
	/*
	itc= contours.begin();

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// 중심점 그리기
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	while (itc!=contours.end()) {

		//상태바에 추적점과 contour사이지를 나태내어 주기 위해 메인프레임의 포인터를 얻어온다.
		CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
		//상태바에 정보표시를 하기위한 변수들
	
		// compute all moments
		cv::Moments mom= cv::moments(cv::Mat(*itc));
		
		//중심점 그리기
		cv::Point pt(mom.m10/mom.m00 , mom.m01/mom.m00); 
		// draw mass center
			cv::circle(result,
			// position of mass center converted to integer
			pt,
			2,cv::Scalar(0,255,255),2); // draw dot
		
	
		//상태바에 물체의 중심점 나타내기
		if(pt.x >=0 && pt.y >=0 )
		{
			strText.Format(_T("( %d, %d)"), pt.x, pt.y);
			pMainFrame->m_wndStatusBar.SetPaneText(0,strText);
		}
		
		//상태바에 물체의 사이즈 나타내기
		if(itc->size() ){
			strText2.Format(_T(" %d"), object_size );
			pMainFrame->m_wndStatusBar.SetPaneText(1,strText2);
		}
		
		//중심점의 x,y좌표와 물체의 사이즈를 avr로 전송
		ColorDetector::Transfer(pt , object_size );
		itc++;

	}
	*/


	//좀더 선명한 결과를 위해
	//2011.8.6
	// Mopology operation 에 대해..

	//Eroding can eliminate very small objects("noisy");
	//cv::Mat element(3,3,CV_8U, cv::Scalar(1)); //3x3 structuring element is applied
	//cv::erode(result,eroded,element); /// in-place processing
	
	//anthor way
	//Eliminate noise and smaller objects
	//cv::erode(result, result, cv::Mat(), cv:Point(-1,-1,),3);
	//cv::Point(-1,-1) means that the origin is at the center of the matrix(default)

	// Dilating can larger some of the "holes" 
	// Opening is defined as the dilation of the erosion of an image
	// Opening filter removes the small blobs(작은 색 부분들) 


	//cv::Mat element5(5,5,CV_8U,cv::Scalar(1));
	//these operators are said to be idempotent.

	//cv::morphologyEx(out, out , cv::MORPH_OPEN,element5);
	//cv::getStructuringElement( MORPH_RECT , Size( 3, 3 ), Point( 1,1 ) ) );
