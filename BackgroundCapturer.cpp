#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {

	// http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture	
	
	// Open the default camera
	VideoCapture cap(0); 
	// Check if we succeeded
	if(!cap.isOpened()) return -1;

	Mat edges;
	namedWindow("edges", 1);

	for(;;) {
		Mat frame;
		// Get a new frame from camera
		cap >> frame;
		cvtColor(frame, edges, CV_BGR2GRAY);
		imshow("edges", edges);
		if(waitKey(30) >= 0) break;
	}
	
	// Camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
