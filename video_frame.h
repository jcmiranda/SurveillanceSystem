#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <time.h>
#include <opencv2/opencv.hpp>
#include <pthread.h>

// TODO: naming conventions? underscores?
typedef struct VideoFrame {
    // Image data associated with this frame 
    cv::Mat frame;    
    // Time of frame capture 
    time_t timestamp;
    // Lock for data within this videoframe (timestamp and
    // frame data) 
    pthread_rwlock_t* rw_lock;

    bool exit_thread;
} VideoFrame_t;

#endif
