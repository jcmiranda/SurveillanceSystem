#ifndef BGD_CAPTURER_SINGLE_H
#define BGD_CAPTURER_SINGLE_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "FrameProcessor.h"
#include "video_frame.h"

class BgdCapturerSingle : public FrameProcessor {
public:
	BgdCapturerSingle(std::vector<VideoFrame_t>* frame_buffer,
           int buffer_length, int frame_width, int frame_height) : 
        FrameProcessor(frame_buffer, buffer_length, 
                frame_width, frame_height) {};
	virtual bool runInThread();
    virtual bool processFrame();
};

#endif
