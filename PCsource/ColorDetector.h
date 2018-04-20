#pragma once

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

class ColorDetector
{

private:

	//������ ������ ���� �ӽ����� ����
	char temp[8];
		
	//minimum acceptable distance
	int minDist;

	//��ü�� ũ��
	int object_size;

	//target color
	cv::Vec3b target;

	//image containing result
	cv::Mat result;
	cv::Mat binary;

	//��ü�� �׸��� �簢��
	cv::Rect object_rect;

	CString strText;		// �߽����� x,y ��ǥ
	CString strText2;		// ���� ��ü�� ũ��

	//inline private memeber function
	//Computes the distance from target color
	int getDistance(const cv::Vec3b& color)const {
		
		//There is two methods for this function
		
		//1. Using OpenCV function
		 return static_cast<int>(
			 cv::norm<int,3>(cv::Vec3i(color[0]-target[0],
									   color[1]-target[1],
									   color[2]-target[2])));
		
		//2. simple computation 
		/*return abs(color[0] - target[0]) +
				abs(color[1] - target[1]) +
				abs(color[2] - target[2]);*/
	}

public:
	// empty constructor
	ColorDetector() : 
	minDist(100),object_size(0){
		//default parameter initializtion here
		target[0] = target[1] = target[2] = 0;
	}

	// destroyer
	~ColorDetector(void){}

	//Getters and setters

	//Sets the color distance threshold
	//Threshold must be positive, otherwise distance threshold is set to 0.

	void setColorDistanceThreshold(int distance){
		if(distance <0)
			distance = 0;
		minDist = distance;
	}

	//Gets the color distance threshold
	int getColorDistanceThreshold() const{

		return minDist;
	}

	//Sets the color to be detected
	void setTargetColor(unsigned char red, 
						unsigned char green, 
						unsigned char blue){
		//BGR order
		target[2] = red;
		target[1] = green;
		target[0] = blue;
	}

	//Set the color to be detected
	void setTargetColor(cv::Vec3b color) {

		target = color;
	}

	//AVR�� ������ ������ ���� �Լ�
	void Transfer(cv::Point pt , int size );

	//Gets the color to be detected
	cv::Vec3b getTargetColor() const {
		return target;
	}

	//Processes the image.
	cv::Mat process(const cv::Mat &image);
};

