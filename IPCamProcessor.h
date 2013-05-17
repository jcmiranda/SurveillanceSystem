#ifndef IP_CAM_PROCESSOR_H
#define IP_CAM_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <string>
#include <pthread.h>

class IPCamProcessor {
    public:
        IPCamProcessor(int frame_width, int frame_height) :
            _frame_width(frame_width),
            _frame_height(frame_height),
            _frame(cv::Mat(frame_height, frame_width, CV_8UC1, cv::Scalar(0))) {
                int rc = 0;
                if( (rc = pthread_rwlock_init(&_frame_lock, NULL)) != 0) {
                    perror("rwlock initialization failed in FrameProcessor constructor.");
                }

            };
        ~IPCamProcessor() {
            pthread_rwlock_destroy(&_frame_lock);
        }

        bool runInThread();
        bool getFrame(cv::Mat* frame_buffer);
    private:
        int _frame_width;
        int _frame_height;
        pthread_rwlock_t _frame_lock;
        cv::Mat _frame;
};
#endif
