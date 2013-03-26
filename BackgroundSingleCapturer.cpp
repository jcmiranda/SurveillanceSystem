#include "BackgroundSingleCapturer.h"

#include <iostream>
	
// http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture	

using namespace cv;

BackgroundSingleCapturer::BackgroundSingleCapturer(const std::string &bgd_filename,
		cv::VideoCapture* cap, cv::Mat* bgd, pthread_mutex_t* mutex_bgd) : 
	_bgd_filename(bgd_filename),
	_bgd(bgd),
	_mutex_bgd(mutex_bgd),
	_cap(cap) {
}

bool BackgroundSingleCapturer::runInThread() {
	// Check if camera is open
	if(!_cap->isOpened()) return false;

	std::cout << "Press enter to capture background frame." << std::endl;	
	// Capture frame on keyboard input
	char* line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	linelen = getline(&line, &linecap, stdin);

	Mat frame;
	// Get a new frame from camera
	*(_cap) >> frame;
	cvtColor(frame, frame, CV_BGR2GRAY);
	if(!imwrite(_bgd_filename,frame)) {
		perror("Unable to write background to file.");
		return false;
	}
	return true;
}

