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
}

bool BackgroundSingleCapturer::captureBackground() {
	// Check if camera is open
	if(!_cap.isOpened()) return false;


	std::cout << "Press enter to capture background frame." << std::endl;	
	// Capture frame on keyboard input
	char* line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	linelen = getline(&line, &linecap, stdin);

	Mat edges;
	Mat frame;
	// Get a new frame from camera
	_cap >> frame;
	cvtColor(frame, edges, CV_BGR2GRAY);
	// Camera will be deinitialized automatically in VideoCapture destructor
	std::cout << "Writing background file." << std::endl;	
	return imwrite(_filename, edges);
}

