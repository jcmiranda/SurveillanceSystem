#include <time.h>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <vector>

#include "video_frame.h"

const static int FRAME_HEIGHT = 240;
const static int FRAME_WIDTH = 360;
const static int FRAME_BUFLEN = 100;
static int cur_frame_i = 0;
static std::vector<cv::Mat> frame_buffer(sizeof(cv::Mat));

int main(int argc, char** argv) {

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

