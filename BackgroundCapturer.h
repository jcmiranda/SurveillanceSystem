#ifndef BACKGROUNDCAPTURER_H
#define BACKGROUNDCAPTURER_H

#include <opencv2/opencv.hpp>
#include <string>
#include <pthread.h>
#include <vector>

class BackgroundCapturer {
public:
	BackgroundCapturer(const std::string &bgd_filename,
			std::vector<cv::Mat>* frame_buffer,
			//cv::VideoCapture* cap, 
			cv::Mat* bgd, 
			pthread_mutex_t* mutex_bgd,
			pthread_mutex_t* mutex_r_frame_i);
	virtual bool runInThread() = 0;
	// bool setRFrameI(int r_frame_i);
	// int getRFrameI(); // May be unneeded
protected:
	const std::string _bgd_filename;
	std::vector<cv::Mat>* _frame_buffer;
	cv::Mat* _bgd;
	pthread_mutex_t* _mutex_bgd;
	pthread_mutex_t* _mutex_r_frame_i;
	// int _r_frame_i;
	// cv::VideoCapture* _cap;
};

#endif
