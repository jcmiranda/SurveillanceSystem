#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <time.h>
#include <opencv2/opencv.hpp>
#include <pthread.h>

typedef struct VideoFrame {
  cv::Mat frame;
  time_t timestamp;
} VideoFrame_t;

#endif
