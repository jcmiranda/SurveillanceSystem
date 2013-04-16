#include "MotionProbYDiff.h"

bool MotionProbYDiff::getMotionProbs(const cv::Mat& frame, 
        const cv::Mat& bgd,
        cv::Mat* mask) {
       
    cv::absdiff(frame, bgd, *mask);
   
    return true;
}
