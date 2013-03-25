#ifndef BACKGROUNDSINGLECAPTURER_H
#define BACKGROUNDSINGLECAPTURER_H

#include "BackgroundCapturer.h"
#include <string>
#include <opencv2/opencv.hpp>

class BackgroundSingleCapturer : public BackgroundCapturer {
public:
	BackgroundSingleCapturer(const std::string &filename);
	virtual bool captureBackground();
private:
	const std::string _filename;
	cv::VideoCapture _cap;
};

#endif
