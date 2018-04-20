#pragma once

 

#include "ColorDetector.h"

class ColorDetectController
{
private:
	//pointer to the singleton
	static ColorDetectController *singleton;

	// the algorithm class
	ColorDetector *cdetect;
	cv::Mat image;		//The image to be processed
	cv::Mat result;
public:
	ColorDetectController(){

		//setting up the application
		cdetect = new ColorDetector();
	}

	//Sets the color distance threshold
	void setColorDistanceThreshold(int distance){
		
		cdetect->setColorDistanceThreshold(distance);
	}

/*	//Gets the color distance threshold
	int getColorDistanceThreshold() const {
		
		return cdetect->getColorDistanceThreshold();
	}
	
	*/
	//Sets the color to be detected
	void setTargetColor(unsigned char red,unsigned char green, unsigned char blue){

			cdetect->setTargetColor(red,green,blue);
	}

/*	//Gets the color to be detected
	void getTargetColor(unsigned char &red,
		unsigned char &green, unsigned char &blue) const{
		
			cv::Vec3b color = cdetect->getTargetColor();

			red = color[2];
			green = color[1];
			blue = color[0];
	}
	*/
	//Sets the input image, Reads it from Video.
	bool setInputImage(cv::Mat& img){
		image = img;
		if(!image.data)
			return false;
		else
			return true;
	}

	//Retrurns the current input image.
	const cv::Mat getInputImage() const{
		return image;
	}

	//Performs image process
	void process(){
		
		result = cdetect->process(image);
	}

	//Returns the image result from the latest processing.
	const cv::Mat getLastResult() const {

		return result;
	}

	//Deletes processor objects created by the controller
	~ColorDetectController(){
		delete cdetect;
	}

	//Singleton static members
	static ColorDetectController *getInstance(){
		//Creates the instance at first call

		if(singleton ==0)
			singleton = new ColorDetectController;

		return singleton;
	}

	//Release the singleton intance of this controller
	static void destroy(){

		if(singleton != 0) {
			delete singleton;
			singleton = 0;
		}
	}

};

