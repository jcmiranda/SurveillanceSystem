#ifndef MOTIONDETECTOR_H
#define MOTIONDETECTOR_H

#include <opencv2/opencv.hpp>

class MotionDetector {
public:
	MotionDetector(cv::Mat* bgd,
			pthread_mutex_t* mutex_bgd);
	int getRFrameI();
	virtual bool runInThread() = 0;
protected:
	cv::VideoCapture* _cap;
	cv::Mat* _bgd;
	pthread_mutex_t* _mutex_bgd;
	// Index of frame currently being read by motion detector
	int _r_frame_i;
};

#endif

