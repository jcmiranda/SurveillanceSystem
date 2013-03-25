#include "BackgroundSingleCapturer.h"
#include <string>
#include <opencv2/opencv.hpp>
#include <iostream>
	
// http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture	

using namespace cv;

BackgroundSingleCapturer::BackgroundSingleCapturer(const std::string &filename) : 
	_filename(filename) {
	VideoCapture cap(0);
	_cap = cap;
	// _cap = new cv::VideoCapture(0);
}

bool BackgroundSingleCapturer::captureBackground() {
	// std::cout << "Before opening video capture" << std::endl;
	// Open the default camera
	// VideoCapture cap(0);
    // std::cout << "after initializing video capture" << std::endl;	
	// Check if we succeeded
	if(!_cap.isOpened()) return false;
	
	std::cout << "After opening video capture" << std::endl;

	Mat edges;
	// namedWindow("edges", 1);
	
	// Capture last frame
	// for(;;) {
		Mat frame;
		// Get a new frame from camera
		_cap >> frame;
		cvtColor(frame, edges, CV_BGR2GRAY);
		// imshow("edges", edges);
		// if(waitKey(30) >= 0) break;
	//}
	
	// Camera will be deinitialized automatically in VideoCapture destructor
	std::cout << "Writing background file." << std::endl;	
	return imwrite(_filename, edges);
}

