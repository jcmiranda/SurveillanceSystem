#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <time.h>
#include <opencv2/opencv.hpp>
#include <pthread.h>

// TODO: naming conventions? underscores?
typedef struct VideoFrame {
    // Image data associated with this frame 
    cv::Mat frame;    
    
    cv::Mat color_frame;

    // Image data associated with ip camera frame for this
    // iteration
    cv::Mat ip_frame;
    
    cv::Mat color_ip_frame;

    // Time of frame capture 
    time_t timestamp;
    // Lock for data within this videoframe (timestamp and
    // frame data) 
    pthread_rwlock_t* rw_lock;
    // Variable to signal that a thread reading from this
    // frame buffer should exit in order to join main thread
    bool exit_thread;
} VideoFrame_t;

#endif
