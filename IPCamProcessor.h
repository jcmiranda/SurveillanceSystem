#ifndef IP_CAM_PROCESSOR_H
#define IP_CAM_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "video_frame.h"
#include "FrameProcessor.h"

class IPCamProcessor : public FrameProcessor {
    public:
        IPCamProcessor(std::vector<VideoFrame_t>* frame_buffer,
                int buffer_length, 
                int frame_width, 
                int frame_height) :
            FrameProcessor(frame_buffer, buffer_length,
                    frame_width, frame_height), 
            _last_pair(cv::Mat(frame_height, 
                        2*frame_width, 
                        CV_8UC1, 
                        cv::Scalar(0))) {
                int rc = 0;
                if( (rc = pthread_rwlock_init(&_last_pair_lock, 
                                NULL)) != 0) {
                    perror("rwlock initialization failed in ip cam processor constructor.");
                }
            };

        ~IPCamProcessor() {
            pthread_rwlock_destroy(&_last_pair_lock);
        };
        
        virtual bool processFrame();
        bool getLastPair(cv::Mat* dst);
        
    private:
        cv::Mat _last_pair;

        pthread_rwlock_t _last_pair_lock;
};

#endif // IP_CAM_PROCESSOR_H

