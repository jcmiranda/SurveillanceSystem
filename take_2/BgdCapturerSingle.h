#ifndef BGD_CAPTURER_SINGLE_H
#define BGD_CAPTURER_SINGLE_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "BgdCapturer.h"
#include "video_frame.h"

class BgdCapturerSingle : BgdCapturer {
public:
	BgdCapturerSingle(std::vector<VideoFrame_t>* frame_buffer);
	virtual bool runInThread() = 0;
    virtual bool getBgd(cv::Mat* bgd_buffer) = 0;
protected:
	std::vector<VideoFrame_t>* _frame_buffer;
};

#endif
