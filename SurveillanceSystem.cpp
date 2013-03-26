#include "BackgroundSingleCapturer.h"
#include <pthread.h>
#include <iostream>
#include <opencv2/opencv.hpp>

const static int FRAME_HEIGHT = 480;
const static int FRAME_WIDTH = 720;

void* capture_background(void* arg) {
	BackgroundSingleCapturer backgroundSingleCapturer =
		*((BackgroundSingleCapturer*) arg);
	if(!backgroundSingleCapturer.runInThread()) {
		perror("Error capturing background");
		return NULL;
	}
	return NULL;	
}

int main(int argc, char** argv) {
	
	// Capture default webcam feed
	cv::VideoCapture video_cap(0);
	if(!(video_cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH) &
				video_cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT))) {
		perror("Can not set frame width and height");
	}

	// Background frame
	cv::Mat bgd_frame(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1, cv::Scalar(0));
	
	// Mutex for editing background frame
	pthread_mutex_t mutex_bgd;
	if(pthread_mutex_init(&mutex_bgd, NULL) != 0) {
		perror("Failed to initialize background mutex");
		return -1;
	}

	// Filename for saving current background
	std::string bgd_filename = "background.jpg";

	// Intialize background capturing option
	BackgroundSingleCapturer backgroundSingleCapturer(bgd_filename,
			&video_cap, &bgd_frame, &mutex_bgd);

	// Start thread for capturing background
	pthread_t background_capture_thread;
	if(pthread_create(&background_capture_thread, 
				NULL, 
				&capture_background, 
				&backgroundSingleCapturer)) {
		perror("Could not create thread to capture background.");
		return  -1;
	}

	// Display live video feed window
	cv::namedWindow("livefeed", 1);	
	for(;;) {
		cv::Mat cur_frame;
		cv::Mat frame_and_bgd(FRAME_HEIGHT, 2*FRAME_WIDTH, CV_8UC1, cv::Scalar(0));

		int return_val = pthread_mutex_trylock(&mutex_bgd);
		video_cap >> cur_frame; // Get a new frame from camera
		cvtColor(cur_frame, cur_frame, CV_BGR2GRAY);
		if(return_val != 0) {
			hconcat(cur_frame, 
					cv::Mat(FRAME_HEIGHT, 
						FRAME_WIDTH, 
						CV_8UC1, 
						cv::Scalar(0)), 
					frame_and_bgd);
		} else {
			hconcat(cur_frame, bgd_frame, frame_and_bgd);
			return_val = pthread_mutex_unlock(&mutex_bgd);
			if(return_val != 0) {
				std::cout << "Failed unlocking bgd" << std::endl;
			}	
		}

		// TODO: is this interfering with similar line in background
		// single capture?
		cv::imshow("livefeed", frame_and_bgd);
		if(cv::waitKey(30) >= 0) break;
	}

	// Join thread for capturing background	
	if(pthread_join(background_capture_thread, NULL)) {
		perror("Could not join background capture thread");
		return -1;
	}

	// Destroy mutex for background
	if(pthread_mutex_destroy(&mutex_bgd) != 0) {
		perror("Failed to destroy background mutex");
		return -1;
	}

	return 0;	
}
