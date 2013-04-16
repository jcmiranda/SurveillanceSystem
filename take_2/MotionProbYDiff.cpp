#include "MotionProbYDiff.h"

bool MotionProbYDiff::getMotionProbs(const cv::Mat& frame, 
        const cv::Mat& bgd,
        cv::Mat* mask) {
        
    (*mask) = cv::abs(frame - bgd);
    
    return true;
}
