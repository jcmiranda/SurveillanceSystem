#include <opencv2/opencv.hpp>
#include <string>

// to compile
// g++ opencv_ipcam_stream.c -o output -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_video -I/opt/local/include -Wall

const static int FRAME_HEIGHT = 240;
const static int FRAME_WIDTH = 370;

int main(int argc, char** argv) {
   
    const std::string videoStreamAddress = "http://192.168.2.30/video.mjpg";
    // Capture default webcam feed
    cv::VideoCapture video_cap;
    cv::Mat image;

    if(!video_cap.open(videoStreamAddress)) {
        std::cout << "error opening video stream or file" << std::endl;
        return -1;
    }

    std::cout << "after opening video stream" << std::endl;

    for(;;) {
        if(!video_cap.read(image)) {
            std::cout << "no frame" << std::endl;
            cv::waitKey();
        }

        cv::imshow("Output window", image);
        if(cv::waitKey(1) >= 0) break;
    }

}

