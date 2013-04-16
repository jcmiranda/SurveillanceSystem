#include "BgdCapturerSingle.h"
#include "video_frame.h"

// No running in thread for a single capture - listening on stdin
// is done in main thread to prevent multiple things from trying to read
// from same IO buffer
bool BgdCapturerSingle::runInThread() {
    return true;
}

// Nothing to do on frame process for single capture
bool BgdCapturerSingle::processFrame() {
    return true;
}
