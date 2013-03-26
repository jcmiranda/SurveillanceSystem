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

	if(pthread_mutex_lock(_mutex_bgd) != 0) {
		perror("Can not acquire lock on background");
		return false;
	}

	*(_cap) >> *_bgd;
	cvtColor(*_bgd, *_bgd, CV_BGR2GRAY);
	if(!imwrite(_bgd_filename, *_bgd)) {
		perror("Unable to write background to file.");
		if(pthread_mutex_unlock(_mutex_bgd) != 0) {
			perror("Failed to unlock mutex after image write fail");
		}
		return false;
	}
	
	if(pthread_mutex_unlock(_mutex_bgd) != 0) {
		perror("Can not unlock on background");
		return false;
	}
	
	return true;
}

