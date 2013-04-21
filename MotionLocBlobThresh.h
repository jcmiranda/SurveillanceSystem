#ifndef MOTION_LOC_BLOB_THRESH_H
#define MOTION_LOC_BLOB_THRESH_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "video_frame.h"
#include "FrameProcessor.h"
#include "MotionProbYDiff.h"
#include "cvblob.h"

class MotionLocBlobThresh : public FrameProcessor {
    public:
        MotionLocBlobThresh(std::vector<VideoFrame_t>* frame_buffer,
                int buffer_length, 
                int frame_width, 
                int frame_height) :
            FrameProcessor(frame_buffer, buffer_length,
                    frame_width, frame_height), 
            _motion_prob_y_diff(frame_width, frame_height),
            _last_prob_mask(cv::Mat(frame_height, 
                        frame_width, 
                        CV_8UC1, 
                        cv::Scalar(0))) {
                int rc = 0;
                if( (rc = pthread_rwlock_init(&_motion_blobs_lock, 
                                NULL)) != 0) {
                    perror("rwlock initialization failed in motion locator constructor.");
                }
                if( (rc = pthread_rwlock_init(&_last_prob_mask_lock, 
                                NULL)) != 0) {
                    perror("rwlock prob mask initialization failed in motion locator constructor.");
                }

                _label_img = cvCreateImage(cvSize(frame_width,
                            frame_height), IPL_DEPTH_LABEL,
                        1);
            };

        ~MotionLocBlobThresh() {
            pthread_rwlock_destroy(&_motion_blobs_lock);
            pthread_rwlock_destroy(&_last_prob_mask_lock);
        };
        
        virtual bool processFrame();
        bool getLastProbMask(cv::Mat* dst);
        
        // Returns cvb::CvBlobs object with the blobs for all motion
        bool getLastMotionBlobs(cvb::CvBlobs* blobs);
        bool annotateMatWithBlobs(cv::Mat* mat);
        
        bool findMaxLocation(cv::Mat mask,
               int num_locations, 
               cv::Point* dst_loc,
               cv::Point* dst_loc2); 
    private:
        MotionProbYDiff _motion_prob_y_diff;
        cv::Mat _last_prob_mask;
        cvb::CvBlobs _motion_blobs;
        IplImage* _label_img;

        pthread_rwlock_t _last_prob_mask_lock;
        // Lock for _motion_blobs and _label_img
        pthread_rwlock_t _motion_blobs_lock;
};

#endif // MOTION_LOC_BLOB_THRESH_H
