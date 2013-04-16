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
            _motion_prob_y_diff(frame_width, frame_height) {
                int rc = 0;
                if( (rc = pthread_rwlock_init(&_motion_centers_lock, NULL)) != 0) {
                    perror("rwlock initialization failed in motion locator constructor.");
                }
            };
        ~MotionLocatorGrid() {
            pthread_rwlock_destroy(&_motion_centers_lock);
        };
        virtual bool processFrame();
        cv::Point getMotionCenters();
        cv::Point findMaxLocation(cv::Mat mask,
               int num_locations); 
    private:
        MotionProbYDiff _motion_prob_y_diff;
        cv::Point _motion_center;
        pthread_rwlock_t _motion_centers_lock;
};

#endif // MOTION_LOCATOR_GRID_H
