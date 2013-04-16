#include "MotionLocatorGrid.h"

bool MotionLocatorGrid::processFrame() {
    cv::Mat mask(_frame_height, _frame_width, CV_8UC1,
            cv::Scalar(0));
    cv::Mat bgd(_frame_height, _frame_width, CV_8UC1,
            cv::Scalar(0));
    getBgd(&bgd);
    VideoFrame_t& this_frame = (*_frame_buffer)[_cur_frame_i];
    
    // Calculate motion probabilities
    _motion_prob_y_diff.getMotionProbs(this_frame.frame, 
            bgd, 
            &mask);

    std::cout << "finish writing calculation of motion locations for probabilities." << std::endl;

    // Calculate highest diff point of motion probabilties
    return true;
}
