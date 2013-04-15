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
	// Initialize frame width and frame height for frame capture
    if(!(video_cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH) &
				video_cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT))) {
		perror("Can not set frame width and height");
	}
  
   // Initialize frame buffer 
    for(int i = 0; i < FRAME_BUFLEN; i++) {
        video_frame_buffer[i].frame = cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1, cv::Scalar(0));
        video_frame_buffer[i].timestamp = time_t();
        pthread_rwlock_t* rw_lock = (pthread_rwlock_t*) malloc(sizeof(pthread_rwlock_t));
        // TODO: check attr for initialization
        pthread_rwlock_init(rw_lock, NULL);
      video_frame_buffer[i].rw_lock = rw_lock; 
    } 
	
    // Display live video feed window
	cv::namedWindow("livefeed", 1);	

    for(;;) {
        video_cap >> video_frame_buffer[cur_frame_i].frame; 
        cvtColor(video_frame_buffer[cur_frame_i].frame, 
                video_frame_buffer[cur_frame_i].frame, CV_BGR2GRAY);
       
        time(&video_frame_buffer[cur_frame_i].timestamp);
		cv::imshow("livefeed", video_frame_buffer[cur_frame_i].frame);
        
        cur_frame_i = (cur_frame_i + 1) % FRAME_BUFLEN;

        if(cv::waitKey(30) >= 0) break;
    } 
  
   // TOOD: memory cleanup for rwlocks 
    for(int i = 0; i < FRAME_BUFLEN; i++) {
        pthread_rwlock_destroy(video_frame_buffer[i].rw_lock);
    }

    video_cap.release();
    cv::destroyWindow("livefeed");
    return 0;
}

