#include "MotionLocatorGrid.h"

#include <vector>
#include <opencv2/opencv.hpp>

std::vector<cv::Point> findMaxLocations(cv::Mat mask, int num_locations) {
    
    
    std::vector<cv::Point> max_locations(num_locations, cv::Point(0, 0));
  
    int histSize = 256; 
    float range[] = {0, 256};
    const float* histRange = {range};
    bool uniform = true;
    bool accumulate = false;

    cv::Mat y_hist;
    cv::calcHist(&mask, 1, 0, y_hist, 1, &histSize, uniform, accumulate);

    for (int i = histSize - 1; i--; i>= 0) {
        if (y_hist.at<float>(i) > 0) {
            std::cout << "i: " << i << " num: " << int(y_hist.at<float>(i) << std::endl;
        }
    }

    /*
    double max_val; double min_val;
    cv::Point max_loc; cv::Point min_loc;

    cv::Mat ignore = cv::Mat(frame_height, frame_width, );
    for(int i = 0; i < num_locations; i++) {
        cv::minMaxLoc(mask, 
                &min_val, 
                &max_val,
                &min_loc, 
                &max_loc,
                ignore); 
        
        max_locations[i].x = max_loc.x;
        max_locations[i].y = max_loc.y;

        std::cout << max_locations[i] << ": " 
            << int(mask.at<unsigned char>(max_locations[i].x, max_locations[i].y))
            << std::endl;

        // TODO: figure out some way to not stomp on data
        ignore.at<int>(max_locations[i].x, 
                max_locations[i].y) = 1;
    }

    // std::cout << mask << std::endl;
*/
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


