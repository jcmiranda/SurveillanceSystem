#include <time.h>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <vector>

#include "video_frame.h"

// Height and width of frame in pixels
const static int FRAME_HEIGHT = 240;
const static int FRAME_WIDTH = 360;

// Length of video frame buffer
const static int FRAME_BUFLEN = 100;

// The index to which the current frame is being written
static int cur_frame_i = 0;
static std::vector<VideoFrame_t> video_frame_buffer(sizeof(VideoFrame_t));

int main(int argc, char** argv) {
    // Capture default webcam feed
    cv::VideoCapture video_cap(0);
    VideoFrame_t video_frame;
    video_frame.frame = cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1, cv::Scalar(0));
    video_frame.timestamp = time_t();
    for (int i = 0; i < 10; i++) {
        time(&video_frame.timestamp);
        std::cout << video_frame.timestamp << std::endl;
        sleep(1);
    }
    return 0;
}

