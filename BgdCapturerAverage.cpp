#include "BgdCapturerAverage.h"
#include "video_frame.h"

// When process frame is called, this thread holdes rd locks on
// _cur_frame_i and _cur_frame_i + 1
bool BgdCapturerAverage::processFrame() {
    _ctr = (_ctr + 1) % _step;
    if (_ctr == 0) {
        // TODO: process this frame for averaging
        std::cout << "No average function implemented." << std::endl;
    }
    return true;
}
