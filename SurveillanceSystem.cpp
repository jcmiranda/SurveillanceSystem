#include "BackgroundSingleCapturer.h"

int main(int argc, char** argv) {
	const std::string background_file = "background.jpg";
	BackgroundSingleCapturer backgroundSingleCapturer(background_file);

	backgroundSingleCapturer.captureBackground();
	
	return 0;	
}
