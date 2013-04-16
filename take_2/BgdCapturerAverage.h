#ifndef BGD_CAPTURER_SINGLE_H
#define BGD_CAPTURER_SINGLE_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "FrameProcessor.h"
#include "video_frame.h"

class BgdCapturerAverage : public FrameProcessor {
public:
	BgdCapturerAverage(std::vector<VideoFrame_t>* frame_buffer,
           int buffer_length, int frame_width, int frame_height) : 
        FrameProcessor(frame_buffer, buffer_length, 
                frame_width, frame_height),
   _ctr(0), _step(120) {};
	virtual bool runInThread();
    virtual bool processFrame() = 0;
private:
    int _ctr;
    int _step;
};

#endif
