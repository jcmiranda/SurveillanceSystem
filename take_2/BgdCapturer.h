#ifndef BGD_CAPTURER_H
#define BGD_CAPTURER_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "video_frame.h"

class BgdCapturer {
public:
	BgdCapturer(std::vector<VideoFrame_t>* frame_buffer) :
        _frame_buffer(frame_buffer) {}; 
	virtual bool runInThread() = 0;
    virtual bool getBgd(cv::Mat* bgd_buffer) = 0;
protected:
	std::vector<VideoFrame_t>* _frame_buffer;
    int _cur_frame_i;
};

#endif
