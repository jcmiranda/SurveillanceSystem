#include <time.h>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <vector>

#include "video_frame.h"
#include "BgdCapturerSingle.h"
#include "MotionLocatorGrid.h"

// Height and width of frame in pixels
const static int FRAME_HEIGHT = 240;
const static int FRAME_WIDTH = 370;

// Length of video frame buffer
const static int FRAME_BUFLEN = 100;

// The index to which the current frame is being written
static int cur_frame_i = 0;
static std::vector<VideoFrame_t> video_frame_buffer(sizeof(VideoFrame_t));

void* capture_background(void* arg) {
	BgdCapturerSingle bgdCapturerSingle =
		*((BgdCapturerSingle*) arg);
	if(!bgdCapturerSingle.runInThread()) {
		perror("Error capturing background");
		return NULL;
	}
	return NULL;	
}

void* locate_motion(void* arg) {
	MotionLocatorGrid motionLocatorGrid =
		*((MotionLocatorGrid*) arg);
	if(!motionLocatorGrid.runInThread()) {
		perror("Error locating motion");
		return NULL;
	}
	return NULL;	
}

int main(int argc, char** argv) {
    // Capture default webcam feed
    cv::VideoCapture video_cap(0);
	// Initialize frame width and frame height for frame capture
    if(!(video_cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH) &
				video_cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT))) {
		perror("Can not set frame width and height");
	}
	
   // Return code for initializing rwlocks 
   int rc = 0; 
   // Initialize frame buffer 
    for(int i = 0; i < FRAME_BUFLEN; i++) {
        video_frame_buffer[i].frame = 
            cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1, cv::Scalar(0));
        video_frame_buffer[i].timestamp = time_t();
        pthread_rwlock_t* rw_lock = 
            (pthread_rwlock_t*) malloc(sizeof(pthread_rwlock_t));
        // TODO: check attr for initialization
        if( (rc = pthread_rwlock_init(rw_lock, NULL)) != 0) {
            perror("rwlock initialization failed.");
        }
      video_frame_buffer[i].rw_lock = rw_lock; 
    } 
    
    // Intialize background capturing option
	BgdCapturerSingle bgdCapturerSingle(&video_frame_buffer,
           FRAME_BUFLEN, FRAME_WIDTH, FRAME_HEIGHT);
	
    // Start thread for capturing background
	pthread_t background_capture_thread;
	if(pthread_create(&background_capture_thread, 
				NULL, 
				&capture_background, 
				&bgdCapturerSingle)) {
		perror("Could not create thread to capture background.");
		return  -1;
	}

    // Intialize background capturing option
	MotionLocatorGrid motionLocatorGrid(&video_frame_buffer,
           FRAME_BUFLEN, FRAME_WIDTH, FRAME_HEIGHT);
	
    // Start thread for capturing background
	pthread_t motion_location_thread;
	if(pthread_create(&motion_location_thread,
				NULL, 
				&locate_motion,
				&motionLocatorGrid)) {
		perror("Could not create thread to locate motion.");
		return  -1;
	}
	
    // Display live video feed window
	cv::namedWindow("livefeed", 1);	

    // Stream video
    for(;;) {
        // TODO: double check ordering of this and the lock. Do I want this after the lock?
        const VideoFrame& this_video_frame = video_frame_buffer[cur_frame_i];
      
        // Acquire write lock on this frame
        if( (rc = pthread_rwlock_wrlock(this_video_frame.rw_lock)) != 0) {
            perror ("Failed to acquire write lock on next video frame.");
        }
       
        video_cap >> video_frame_buffer[cur_frame_i].frame; 
        cvtColor(video_frame_buffer[cur_frame_i].frame, 
                video_frame_buffer[cur_frame_i].frame, CV_BGR2GRAY);
       
        time(&video_frame_buffer[cur_frame_i].timestamp);
		cv::imshow("livefeed", video_frame_buffer[cur_frame_i].frame);
       
        // Release write lock on this frame
        if( (rc = pthread_rwlock_unlock(this_video_frame.rw_lock)) != 0) {
            perror ("Failed to release write lock on next video frame.");
        }

        int prev_frame_i = cur_frame_i;
        cur_frame_i = (cur_frame_i + 1) % FRAME_BUFLEN;

        int key = cv::waitKey(30);
        if( (key == 66) | (key == 98)) { // B or b
            if ( (rc = pthread_rwlock_rdlock(
                            video_frame_buffer[prev_frame_i].rw_lock))
                    != 0) {
            perror ("Failed to acquire read lock on video frame to capture as bgd.");
            }

            std::cout << "setting bgd" << std::endl;

            bgdCapturerSingle.
                setBgd(video_frame_buffer[prev_frame_i].frame);
        
            if ( (rc = pthread_rwlock_unlock(
                            video_frame_buffer[prev_frame_i].rw_lock))
                    != 0) {
            perror ("Failed to release read lock on video frame to capture as bgd.");
            }
        } else if (key >= 0) {
            break;
        }
    } 
 
   // TODO: notify background capture thread that it should end
   // TODO: add join for background capture thread
   // TODO: memory cleanup for rwlocks 
    for(int i = 0; i < FRAME_BUFLEN; i++) {
        pthread_rwlock_destroy(video_frame_buffer[i].rw_lock);
        free(video_frame_buffer[i].rw_lock);
    }

    video_cap.release();
    cv::destroyWindow("livefeed");
    return 0;
}
