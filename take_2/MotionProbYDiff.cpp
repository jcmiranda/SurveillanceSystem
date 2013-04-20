#include "MotionProbYDiff.h"

bool MotionProbYDiff::getMotionProbs(const cv::Mat& frame, 
        const cv::Mat& bgd,
        cv::Mat* mask) {
       
    cv::absdiff(frame, bgd, *mask);
    cv::threshold(*mask, *mask, 20, 256, 0); // 0 = binary, 3 - thresh to zero
   
    return true;
}
