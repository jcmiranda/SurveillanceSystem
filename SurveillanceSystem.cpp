#include "BackgroundSingleCapturer.h"
#include <pthread.h>
#include <iostream>

const static std::string background_file = "background.jpg";
// BackgroundSingleCapturer backgroundSingleCapturer(background_file);
static BackgroundSingleCapturer backgroundSingleCapturer(background_file);

void* capture_background(void* arg) {
	// std::cout << "Before casting arg" << std::endl;
	// BackgroundSingleCapturer backgroundSingleCapturer = *((BackgroundSingleCapturer*)(arg));
	// std::cout << "After casting arg" << std::endl;
	if(!backgroundSingleCapturer.captureBackground()) {
		perror("Error capturing background");
		return NULL;
	}
	std::cout << "After capturing background" << std::endl;
	return NULL;	
}

int main(int argc, char** argv) {

	std::cout << "Before starting background capture thread." << std::endl;
	
	// capture_background(NULL);
	pthread_t background_capture_thread;
	if(pthread_create(&background_capture_thread, 
				NULL, 
				&capture_background, 
				NULL)) {
		perror("Could not create thread to capture background.");
		return  -1;
	}
	
	std::cout << "After starting background capture thread." << std::endl;

	if(pthread_join(background_capture_thread, NULL)) {
		perror("Could not join background capture thread");
		return -1;
	}
	
	std::cout << "After joining background capture thread." << std::endl;

	return 0;	
}
