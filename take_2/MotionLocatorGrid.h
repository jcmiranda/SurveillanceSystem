#ifndef MOTION_LOCATOR_GRID_H
#define MOTION_LOCATOR_GRID_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "video_frame.h"
#include "FrameProcessor.h"
#include "MotionProbYDiff.h"

class MotionLocatorGrid : public FrameProcessor {
    public:
        MotionLocatorGrid(std::vector<VideoFrame_t>* frame_buffer,
                int buffer_length, 
                int frame_width, 
                int frame_height) :
            FrameProcessor(frame_buffer, buffer_length,
                    frame_width, frame_height), 
            _motion_prob_y_diff(frame_width, frame_height) {};
        virtual bool processFrame();
    private:
        MotionProbYDiff _motion_prob_y_diff;
};
#endif // MOTION_LOCATOR_GRID_H