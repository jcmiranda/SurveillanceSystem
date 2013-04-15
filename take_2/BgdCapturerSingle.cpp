#include "BgdCapturerSingle.h"
#include "video_frame.h"

// No running in thread for a single capture - listening on stdin
// is done in main thread to prevent multiple things from trying to read
// from same IO buffer
bool BgdCapturerSingle::runInThread() {
    return true;
}

// TODO: implement
bool BgdCapturerSingle::getBgd(cv::Mat* bgd_dest) {
    int rc = 0;
    if ( (rc = pthread_rwlock_rdlock(&_bgd_lock)) != 0) {
        perror("unable to obtain read lock in getBgd");
    }

    _bgd.copyTo(*bgd_dest);
    
    if ( (rc = pthread_rwlock_unlock(&_bgd_lock)) != 0) {
        perror("unable to release read lock in getBgd");
    }

    return true;
}

// Set the bgd to the frame provided as an argument
bool BgdCapturerSingle::setBgd(const cv::Mat& bgd) {
    bgd.copyTo(_bgd);
    return true;
}
