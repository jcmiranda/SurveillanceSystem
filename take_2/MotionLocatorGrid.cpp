#include "MotionLocatorGrid.h"

#include <vector>
#include <opencv2/opencv.hpp>

std::vector<cv::Point> findMaxLocations(cv::Mat mask, int num_locations) {
    std::vector<cv::Point> max_locations(num_locations, cv::Point(-1, -1));
    
    double max_val = -1;
    cv::Point max_loc(-1.0, -1.0);

    for(int i = 0; i < num_locations; i++) {
        cv::minMaxLoc(mask, 
                NULL, 
                &max_val, 
                NULL, 
                &max_loc);
        max_locations[i].x = max_loc.x;
        max_locations[i].y = max_loc.y;

        std::cout << max_locations[i] << ": " 
            << mask.at<cv::Point>(max_locations[i])
            << std::endl;

        // TODO: figure out some way to not stomp on data
        mask.at<float>(max_locations[i].x, max_locations[i].y) = -1.0;
    }

    std::cout << std::endl;

    return max_locations;
}

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

    findMaxLocations(mask, 5);

    // Calculate highest diff point of motion probabilties
    return true;
}


