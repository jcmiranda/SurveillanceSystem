#ifndef MOTION_PROB_H
#define MOTION_PROB_H

#include <opencv2/opencv.hpp>

class MotionProb {
    public:
        MotionProb(int frame_width, int frame_height) :
            _frame_width(frame_width), 
            _frame_height(frame_height) {};
        virtual bool getMotionProbs(const cv::Mat& frame, 
                const cv::Mat& bgd,
                cv::Mat* mask) = 0;
    protected: 
        int _frame_width;
        int _frame_height;
};

#endif // MOTION_PROB_H
