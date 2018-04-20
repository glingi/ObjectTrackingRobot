#pragma once

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace cv;

// The frame processor interface
class FrameProcessor { //vector table

public:
	// processing method
	virtual void process(cv::Mat &input, cv:: Mat &output, int &Threshold, cv::Vec3i &color)= 0;
};

// VideoProcessor

class VideoProcessor
{	
	// The frame processor interface
private:
	// the OpenCV video capture object
	cv::VideoCapture capture;
	// the callback function to be called 
	// for the processing of each frame
	void (*process)(cv::Mat&, cv::Mat&, int&, cv::Vec3i& );
	// the pointer to the class implementing 
	// the FrameProcessor interface
	FrameProcessor *frameProcessor;
	// a bool to determine if the 
	// process callback will be called
	bool callIt;
	// Input display window name
	const char* windowNameInput;
	// Ouyput display window name
	const char*	windowNameOutput;
	// delay between each frame processing
	int delay;
	// to stop the processing
	bool stop;
	//추적할 공의 색
	cv::Vec3i m_nColor;
	//문턱값
	int m_nThreshold;

public:

	 // Constructor setting the default values
	VideoProcessor() : callIt(false), delay(-1), stop(false), 
	 frameProcessor(0), m_nThreshold(120),m_nColor(0,0,255) //B G R
	,windowNameInput("Input Video"),windowNameOutput("Output Video"){}

	//destroyer
	virtual ~VideoProcessor() {}

	// set the camera ID
	bool setInput(int id) {
		// In case a resource was already 
		// associated with the VideoCapture instance
		capture.release();
		
		return capture.open(700+id);
	}

	// set the callback function that will be called for each frame
	void setFrameProcessor(void (*frameProcessingCallback)(cv::Mat&, cv::Mat&, int& Threshold, cv::Vec3i&)) {

		// invalidate frame processor class instance
		frameProcessor= 0;
		// this is the frame processor function that will be called
		process= frameProcessingCallback;
		callProcess();
	}

	// process callback to be called
	void callProcess() {

		callIt= true;
	}

	// do not call process callback
	void dontCallProcess() {

		callIt= false;
	}

	// to display the processed frames
	void displayInput(){
		
		cv::namedWindow(windowNameInput);
		cv::createTrackbar("Threshold", windowNameInput, &m_nThreshold,255,0);
		cv::createTrackbar("RED", windowNameInput, &m_nColor[2],255,0);
		cv::createTrackbar("GREEN", windowNameInput, &m_nColor[1],255,0);
		cv::createTrackbar("BLUE", windowNameInput, &m_nColor[0],255,0);
	}

	// to display the processed frames
	void displayOutput() {
		cv::namedWindow(windowNameOutput);		
	}

	// set a delay between each frame
	// 0 means wait at each frame
	// negative means no delay
	void setDelay(int d) {
	  
		delay= d;
	}
	// Stop the processing
	void stopIt() {

		stop= true;
	}

	// Is the process stopped?
	bool isStopped() {

		return stop;
	}

	// Is a capture device opened?
	bool isOpened() {

		return capture.isOpened();
	}
	  
	// to grab (and process) the frames of the sequence
	void run() {

		// current frame
		cv::Mat frame;
		// output frame
		cv::Mat output;
		// 크기가 조정된 입력
		cv::Mat resizedImage;
	    // 물체 색상 선택을 위한 변수
		cv::Point temp(640,480);
		//입력창 만들기
		displayInput();
		//출력창 만들기
		displayOutput();
		// if no capture device has been set
		if (!isOpened()){
			AfxMessageBox(_T("연결된 카메라가 없습니다."));
			stop = true;
		}
		stop= false;
		
		while ( !isStopped() ) {

			capture >> frame;
		
			//2는 입력프레임의 크기를 조절함.
			cv::resize(frame,resizedImage,cv::Size(frame.cols/2,frame.rows/2) 
				,0,0,INTER_LINEAR);
			//display input frame
			cv::imshow(windowNameInput,resizedImage);
			
		    // calling the process function or method
			if (callIt) {
				  
				// process the frame
				if (process)   //if call back function
					process(resizedImage, output, m_nThreshold, m_nColor ); 
			} else {
				output= frame;
			}

			//출력 화면을 나타내는 창
			cv::imshow(windowNameOutput,output);

			//추적 물체색을 나타내는 창
			cv::Mat ob_color(temp,
								CV_8UC3,
								cv::Scalar(m_nColor[0], m_nColor[1], m_nColor[2]));
			cv::imshow("ObjectColor",ob_color);
			
			// introduce a delay
			if (delay>=0 && cv::waitKey(delay)>=0){
				stopIt();
				cvDestroyAllWindows();
			}
		
		}
	}
};

//Call Back 함수
void ColorBall(cv::Mat& img, cv::Mat& out,int& Threshold, cv::Vec3i& color);
