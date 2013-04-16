#ifndef FRAME_PROCESSOR_H
#define FRAME_PROCESSOR_H 

#include <opencv2/opencv.hpp>
#include <vector>

#include "video_frame.h"

class FrameProcessor {
    public:
        FrameProcessor(std::vector<VideoFrame_t>* frame_buffer,
                int buffer_length, 
                int frame_width, 
                int frame_height) :
            _frame_buffer(frame_buffer),
            _buffer_length(buffer_length),
            _frame_width(frame_width),
            _frame_height(frame_height),
            _bgd(cv::Mat(frame_height, 
                        frame_width, 
                        CV_8UC1, 
                        cv::Scalar(0))), 
            _cur_frame_i(0) {
                int rc = 0;
                if( (rc = pthread_rwlock_init(&_bgd_lock, NULL)) != 0) {
                    perror("rwlock initialization failed in bgd capturer constructor.");
                }
            }; 

        ~FrameProcessor() {
            pthread_rwlock_destroy(&_bgd_lock);
        };

        virtual bool runInThread() = 0;
        virtual bool processFrame() = 0;
        bool getBgd(cv::Mat* bgd_buffer);
        bool setBgd(const cv::Mat& bgd);
    protected:
        // Buffer of video frames that are being updated by the main thread
        std::vector<VideoFrame_t>* _frame_buffer;
        // Length of above buffer
        int _buffer_length;
        // Width and height of frames in the buffer
        int _frame_width;
        int _frame_height;
        // Frame with the bgd data
        cv::Mat _bgd;
        // RW lock for the bgd to ensure concurrency between processing
        // new frames into the bgd and asking for the bgd
        pthread_rwlock_t _bgd_lock;
        // Index of the current frame being processed by the capturer
        // within the videoframe buffer
        int _cur_frame_i;
};
#endif
