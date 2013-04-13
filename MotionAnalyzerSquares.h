#ifndef MOTION_ANALYZER_SQUARES_H
#define MOTION_ANALYZER_SQUARES_H

#include "MotionAnalyzer.h"

class MotionAnalyzerSquares : public MotionAnalyzer {
public:
	MotionAnalyzerSquares(std::vector<cv::Mat>* frame_buffer,
			cv::Mat* bgd, 
			pthread_mutex_t* mutex_bgd,
			pthread_mutex_t* mutex_r_frame_i);
	bool setRFrameI(int r_frame_i);
	int getRFrameI(); 
	virtual bool runInThread();
private:
	int _r_frame_i;
};

#endif

