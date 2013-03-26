#ifndef BACKGROUNDSINGLECAPTURER_H
#define BACKGROUNDSINGLECAPTURER_H

#include "BackgroundCapturer.h"

class BackgroundSingleCapturer : public BackgroundCapturer {
public:
	BackgroundSingleCapturer(const std::string &bgd_filename,
			cv::VideoCapture* cap, 
			cv::Mat* bgd, 
			pthread_mutex_t* mutex_bgd);
	virtual bool runInThread();
protected:
	const std::string _bgd_filename;
	cv::Mat* _bgd;
	pthread_mutex_t* _mutex_bgd;
	cv::VideoCapture* _cap;
};

#endif
