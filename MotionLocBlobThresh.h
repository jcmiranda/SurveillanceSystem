#ifndef MOTION_LOC_BLOB_THRESH_H
#define MOTION_LOC_BLOB_THRESH_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "video_frame.h"
#include "FrameProcessor.h"
#include "MotionProbYDiff.h"

class MotionLocBlobThresh : public FrameProcessor {
    public:
        MotionLocBlobThresh(std::vector<VideoFrame_t>* frame_buffer,
                int buffer_length, 
                int frame_width, 
                int frame_height) :
            FrameProcessor(frame_buffer, buffer_length,
                    frame_width, frame_height), 
            _motion_prob_y_diff(frame_width, frame_height),
            _last_prob_mask(cv::Mat(frame_height, frame_width, CV_8UC1, cv::Scalar(0))) {
                int rc = 0;
                if( (rc = pthread_rwlock_init(&_motion_centers_lock, NULL)) != 0) {
                    perror("rwlock initialization failed in motion locator constructor.");
                }
                if( (rc = pthread_rwlock_init(&_last_prob_mask_lock, NULL)) != 0) {
                    perror("rwlock prob mask initialization failed in motion locator constructor.");
                }
            };
        ~MotionLocBlobThresh() {
            pthread_rwlock_destroy(&_motion_centers_lock);
        };
        virtual bool processFrame();
        cv::Point getMotionCenters(int i);
        bool getLastProbMask(cv::Mat* dst);
        bool findMaxLocation(cv::Mat mask,
               int num_locations, 
               cv::Point* dst_loc,
               cv::Point* dst_loc2); 
    private:
        MotionProbYDiff _motion_prob_y_diff;
        volatile cv::Point _motion_center;
        cv::Point _motion_center_weighted;
        cv::Mat _last_prob_mask;
        pthread_rwlock_t _last_prob_mask_lock;
        pthread_rwlock_t _motion_centers_lock;
};

#endif // MOTION_LOC_BLOB_THRESH_H
