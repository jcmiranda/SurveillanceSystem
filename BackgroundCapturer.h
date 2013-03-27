#ifndef BACKGROUNDCAPTURER_H
#define BACKGROUNDCAPTURER_H

#include <opencv2/opencv.hpp>
#include <string>
#include <pthread.h>

class BackgroundCapturer {
public:
	BackgroundCapturer(const std::string &bgd_filename,
			cv::VideoCapture* cap, 
			cv::Mat* bgd, 
			pthread_mutex_t* mutex_bgd);
	virtual bool runInThread() = 0;
protected:
	const std::string _bgd_filename;
	cv::Mat* _bgd;
	pthread_mutex_t* _mutex_bgd;
	cv::VideoCapture* _cap;
};

#endif
