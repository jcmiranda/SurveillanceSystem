#ifndef BACKGROUNDSINGLECAPTURER_H
#define BACKGROUNDSINGLECAPTURER_H

#include "BackgroundCapturer.h"

class BackgroundSingleCapturer : public BackgroundCapturer {
public:
	BackgroundSingleCapturer(const std::string &bgd_filename,
			std::vector<cv::Mat>* frame_buffer,
			cv::Mat* bgd, 
			pthread_mutex_t* mutex_bgd,
			pthread_mutex_t* mutex_r_frame_i);
	bool setRFrameI(int r_frame_i);
	int getRFrameI(); // May be unneeded
	virtual bool runInThread();
private:
	int _r_frame_i;
};

#endif
