#ifndef IP_CAM_PROCESSOR_H
#define IP_CAM_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "video_frame.h"
#include "MotionLocBlobThresh.h"
#include "FrameProcessor.h"

class IPCamProcessor : public FrameProcessor {
    public:
        IPCamProcessor(std::vector<VideoFrame_t>* frame_buffer,
                int buffer_length, 
                int frame_width, 
                int frame_height,
                MotionLocBlobThresh* motion_loc_blob_thresh) :
            FrameProcessor(frame_buffer, buffer_length,
                    frame_width, frame_height), 
            _last_pair(cv::Mat(frame_height, 
                        2*frame_width, 
                        CV_8UC1, 
                        cv::Scalar(0))),
            _ip_center_x(0), _ip_center_y(0),
            _ip_center_step(_frame_width/15), 
            _ip_radius(_frame_width/5),
   _motion_loc_blob_thresh(motion_loc_blob_thresh) {
                int rc = 0;
                if( (rc = pthread_rwlock_init(&_last_pair_lock, 
                                NULL)) != 0) {
                    perror("rwlock initialization failed in ip cam processor constructor.");
                }

                // _H = cv::Mat(3, 3, CV_32FC1, cv::Scalar(0));
            };

        ~IPCamProcessor() {
            pthread_rwlock_destroy(&_last_pair_lock);
        };
        
        virtual bool processFrame();
        bool getLastPair(cv::Mat* dst);
        
    private:
        cv::Mat _last_pair;
        int _ip_center_x;
        int _ip_center_y;
        // Maximum amount the center of object of the ip camera can move by
        int _ip_center_step;
        int _ip_radius;

        // Homography matrix
        // cv::Mat _H;
        MotionLocBlobThresh* _motion_loc_blob_thresh;
        pthread_rwlock_t _last_pair_lock;
};

#endif // IP_CAM_PROCESSOR_H

