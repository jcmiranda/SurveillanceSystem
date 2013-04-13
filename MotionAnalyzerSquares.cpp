#include "MotionAnalyzerSquares.h"

#include <iostream>
	
using namespace cv;

MotionAnalyzerSquares::MotionAnalyzerSquares(
		const std::string &bgd_filename,
		std::vector<cv::Mat>* frame_buffer,
		cv::Mat* bgd, 
		pthread_mutex_t* mutex_bgd,
		pthread_mutex_t* mutex_r_frame_i) : 
	BackgroundCapturer(bgd_filename, frame_buffer, bgd, mutex_bgd, mutex_r_frame_i) {
}

int MotionAnalyzerSquares::getRFrameI() {
	int return_val = -2;
	if(pthread_mutex_lock(_mutex_r_frame_i) == 0) {
		return_val = _r_frame_i;
		pthread_mutex_unlock(_mutex_r_frame_i);
	} 
	return return_val;
}

// Is not guaranteed to set r_frame_i
// If r_frame_i is not available will not set it
bool MotionAnalyzerSquares::setRFrameI(int r_frame_i) {
	if(pthread_mutex_trylock(_mutex_r_frame_i) == 0) {
		_r_frame_i = r_frame_i;	
		std::cout << "setting r_frame_i" << r_frame_i << " " << _r_frame_i << std::endl;
		pthread_mutex_unlock(_mutex_r_frame_i);
		std::cout << "checking r_frame_i" << getRFrameI() << std::endl;
		return true;
	} 
	std::cout << "Failed to set r_frame_i" << std::endl;
	return false;
}

bool MotionAnalyzerSquares::runInThread() {

  while(true) {  
    if(_r_frame_i >= 0) {
      std::cout << "Acquiring lock on bgd in single capture" << std::endl;
      if(pthread_mutex_lock(_mutex_r_frame_i) != 0) {
        perror("Can not acquire lock on r_frame_i");
        return false;
      }

      if(pthread_mutex_lock(_mutex_bgd) != 0) {
        perror("Can not acquire lock on background");
        return false;
      }

      // insert code here	

      _r_frame_i = -1;

      if(pthread_mutex_unlock(_mutex_r_frame_i) != 0) {
        perror("Can not release lock on r_frame_i");
        return false;
      }

      if(pthread_mutex_unlock(_mutex_bgd) != 0) {
        perror("Can not unlock on background");
        return false;
      }
    }
  }    
  return true;
}


