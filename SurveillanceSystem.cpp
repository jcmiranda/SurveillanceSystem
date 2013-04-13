#include "BackgroundSingleCapturer.h"
#include <pthread.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <vector>

const static int FRAME_HEIGHT = 240;
const static int FRAME_WIDTH = 360;
const static int FRAME_BUFLEN = 25;
static int cur_frame_i = 0;
std::vector<cv::Mat> frame_buffer(sizeof(cv::Mat));

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

	pthread_mutex_t mutex_bgd_r_frame_i;
	if(pthread_mutex_init(&mutex_bgd_r_frame_i, NULL) != 0) {
		perror("Failed to initialize background r frame i mutex");
		return -1;
	}
	
    pthread_mutex_t mutex_motion_r_frame_i;
	if(pthread_mutex_init(&mutex_motion_r_frame_i, NULL) != 0) {
		perror("Failed to initialize motion analysis r frame i mutex");
		return -1;
	}

	// Filename for saving current background
	std::string bgd_filename = "background.jpg";

	// Intialize background capturing option
	BackgroundSingleCapturer backgroundSingleCapturer(bgd_filename,
			&frame_buffer, &bgd_frame, &mutex_bgd, &mutex_bgd_r_frame_i);

    // Initialize motion analysis
    MotionAnalyzerSquares motionAnalyzerSquares(&frame_buffer, &bgd_frame,
        &mutex_bgd, &mutex_motion_r_frame_i);

	// Start thread for capturing background
	pthread_t background_capture_thread;
	if(pthread_create(&background_capture_thread, 
				NULL, 
				&capture_background, 
				&backgroundSingleCapturer)) {
		perror("Could not create thread to capture background.");
		return  -1;
	}

	cv::Mat silhouette(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1, cv::Scalar(0));

	// Display live video feed window
	cv::namedWindow("livefeed", 1);	
	for(;;) {
		// cv::Mat cur_frame;
		cv::Mat frame_and_bgd(FRAME_HEIGHT, 2*FRAME_WIDTH, CV_8UC1, cv::Scalar(0));

		int return_val = pthread_mutex_trylock(&mutex_bgd);
		
		video_cap >> frame_buffer[cur_frame_i]; 
		cvtColor(frame_buffer[cur_frame_i], frame_buffer[cur_frame_i], CV_BGR2GRAY);
		
		if(return_val != 0) {
			hconcat(frame_buffer[cur_frame_i], 
					cv::Mat(FRAME_HEIGHT, 
						FRAME_WIDTH, 
						CV_8UC1, 
						cv::Scalar(0)), 
					frame_and_bgd);
		} else {
			cv::Mat frame_diff;
			frame_diff = cv::abs(frame_buffer[cur_frame_i]- bgd_frame);

			double threshold_value = 30;
			cv::threshold(frame_diff, silhouette, threshold_value, 255, 3);
			hconcat(silhouette, bgd_frame, frame_and_bgd);
			
			return_val = pthread_mutex_unlock(&mutex_bgd);
			if(return_val != 0) {
				std::cout << "Failed unlocking bgd" << std::endl;
			}	
		}
		
		backgroundSingleCapturer.setRFrameI(cur_frame_i);	
        motionAnalyzerSquares.setRFrameI(cur_frame_i);
		cur_frame_i = (cur_frame_i + 1) % FRAME_BUFLEN;	

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

// Motion history code for motionSegment and updateMotionHistory
#if 0
	cv::Mat mhi(FRAME_HEIGHT, FRAME_WIDTH, CV_32FC1, cv::Scalar(0));
	cv::Mat segmask(FRAME_HEIGHT, FRAME_WIDTH, CV_32FC1, cv::Scalar(0));
	double timestamp = (double) clock() / CLOCKS_PER_SEC;
	double duration = 30;
	
	timestamp = (double) clock() / CLOCKS_PER_SEC; 
	std::vector<cv::Rect> boundingRects(sizeof(cv::Rect));
	double segThresh = 40;
	cv::segmentMotion(mhi, segmask, boundingRects, timestamp, segThresh); 
	cv::updateMotionHistory(silhouette, mhi, timestamp, duration);

	// Code for bounding rects is giving entire image	
	for (size_t i = 0; i < boundingRects.size(); i++) {
		cv::Rect this_rect = boundingRects[i];
		cv::rectangle(cur_color_frame, 
				cv::Point(this_rect.x, this_rect.y),
				cv::Point(this_rect.x + this_rect.width, 
					this_rect.y + this_rect.height),
				cv::Scalar(0, 255, 255),
				5, 
				8);	
	}

#endif
