#include "BackgroundSingleCapturer.h"
#include <pthread.h>
#include <iostream>
#include <opencv2/opencv.hpp>

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

	// Background frame
	cv::Mat bgd_frame;
	
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
