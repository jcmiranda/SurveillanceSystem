#include "BgdCapturerSingle.h"
#include "video_frame.h"

BgdCapturerSingle::BgdCapturerSingle(
		std::vector<VideoFrame_t>* frame_buffer)  {
    BgdCapturer(frame_buffer);
}

// TODO: implement
bool BgdCapturerSingle::runInThread() {
    std::cout << "runin thread not implemented" << std::endl;
    return false;
}

// TODO: implement
bool BgdCapturerSingle::getBgd(cv::Mat* bgd_buffer) {
    std::cout << "getbgd not implemented" << std::endl;
    return false;
}
