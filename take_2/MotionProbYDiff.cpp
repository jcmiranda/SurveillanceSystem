#include "MotionProbYDiff.h"

bool MotionProbYDiff::getMotionProbs(const cv::Mat& frame, 
        const cv::Mat& bgd,
        cv::Mat* mask) {
       
    std::cout << "Mask: " << (*mask).type() << std::endl;
    std::cout << "Bgd: " << bgd.type() << std::endl;
    std::cout << "Frame: " << frame.type() << std::endl;

    std::cout << "Frame 0 0: " << frame.at<uint8_t>(0, 0) << std::endl;

    cv::absdiff(frame, bgd, *mask);
   
    return true;
}
