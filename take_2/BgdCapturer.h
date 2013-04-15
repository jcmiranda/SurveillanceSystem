#ifndef BGD_CAPTURER_H
#define BGD_CAPTURER_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "video_frame.h"

class BgdCapturer {
public:
	BgdCapturer(std::vector<VideoFrame_t>* frame_buffer,
            int frame_width, 
            int frame_height) :
        _frame_buffer(frame_buffer),
        _frame_width(frame_width),
        _frame_height(frame_height),
    _bgd(cv::Mat(frame_height, frame_width, CV_8UC1, cv::Scalar(0))) {}; 
    virtual bool runInThread() = 0;
    virtual bool getBgd(cv::Mat* bgd_buffer) = 0;
protected:
	std::vector<VideoFrame_t>* _frame_buffer;
    int _frame_width;
    int _frame_height;
    cv::Mat _bgd;
    int _cur_frame_i;
};

#endif
