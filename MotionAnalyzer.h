#ifndef MOTION_ANALYZER
#define MOTION_ANALYZER

#include <opencv2/opencv.hpp>
#include <string>
#include <pthread.h>
#include <vector>

class MotionAnalyzer {
public:
	BackgroundCapturer(std::vector<cv::Mat>* frame_buffer,
			cv::Mat* bgd, 
			pthread_mutex_t* mutex_bgd,
			pthread_mutex_t* mutex_r_frame_i);
	virtual bool runInThread() = 0;
protected:
	std::vector<cv::Mat>* _frame_buffer;
	cv::Mat* _bgd;
	pthread_mutex_t* _mutex_bgd;
	pthread_mutex_t* _mutex_r_frame_i;
};

#endif

