#include "BackgroundSingleCapturer.h"
#include <string>
#include <opencv2/opencv.hpp>
	
// http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture	

using namespace cv;

BackgroundSingleCapturer::BackgroundSingleCapturer(const std::string &filename) : 
	_filename(filename) {
}

bool BackgroundSingleCapturer::captureBackground() {
	// Open the default camera
	VideoCapture cap(0); 
	// Check if we succeeded
	if(!cap.isOpened()) return -1;

	Mat edges;
	namedWindow("edges", 1);
	
	// Capture last frame
	for(;;) {
		Mat frame;
		// Get a new frame from camera
		cap >> frame;
		cvtColor(frame, edges, CV_BGR2GRAY);
		imshow("edges", edges);
		if(waitKey(30) >= 0) break;
	}
	
	// Camera will be deinitialized automatically in VideoCapture destructor
	
	imwrite(_filename, edges);

	return true;
}

