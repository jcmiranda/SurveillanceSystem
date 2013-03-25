#ifndef BACKGROUNDSINGLECAPTURER_H
#define BACKGROUNDSINGLECAPTURER_H

#include <string>
#include "BackgroundCapturer.h"

class BackgroundSingleCapturer : public BackgroundCapturer {
public:
	BackgroundSingleCapturer(const std::string &filename);
	virtual bool captureBackground();
private:
	const std::string _filename;
};

#endif
