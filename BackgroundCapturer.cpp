#include "BackgroundCapturer.h"

BackgroundCapturer::BackgroundCapturer(
		const std::string &bgd_filename, 
		std::vector<cv::Mat>* frame_buffer, 
		cv::Mat* bgd, 
		pthread_mutex_t* mutex_bgd, 
		pthread_mutex_t* mutex_r_frame_i) : 
	_bgd_filename(bgd_filename),
	_frame_buffer(frame_buffer),
	_bgd(bgd),
	_mutex_bgd(mutex_bgd),
	_mutex_r_frame_i(mutex_r_frame_i)// ,
	// _r_frame_i(-1) 
	{
}

/*
int BackgroundCapturer::getRFrameI() {
	int return_val = -2;
	if(pthread_mutex_lock(_mutex_r_frame_i) == 0) {
		return_val = _r_frame_i;
		pthread_mutex_unlock(_mutex_r_frame_i);
	} 
	return return_val;
}

// Is not guaranteed to set r_frame_i
// If r_frame_i is not available will not set it
bool BackgroundCapturer::setRFrameI(int r_frame_i) {
	if(pthread_mutex_trylock(_mutex_r_frame_i) == 0) {
		_r_frame_i = r_frame_i;	
		std::cout << "setting r_frame_i" << r_frame_i << " " << _r_frame_i << std::endl;
		pthread_mutex_unlock(_mutex_r_frame_i);
		std::cout << "checking r_frame_i" << getRFrameI() << std::endl;
		return true;
	} 
	std::cout << "Failed to set r_frame_i" << std::endl;
	return false;
} */
