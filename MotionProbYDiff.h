#ifndef MOTION_PROB_Y_DIFF_H
#define MOTION_PROB_Y_DIFF_H

#include "MotionProb.h"

#include <opencv2/opencv.hpp>

class MotionProbYDiff : MotionProb {
    public:
        MotionProbYDiff(int frame_width, int frame_height) :
            MotionProb(frame_width, frame_height) {};
        virtual bool getMotionProbs(const cv::Mat& frame, 
                const cv::Mat& bgd,
                cv::Mat* mask);
};

#endif // MOTION_PROB_Y_DIFF_H

