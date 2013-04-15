#include "BgdCapturerSingle.h"
#include "video_frame.h"

// TODO: implement
bool BgdCapturerSingle::runInThread() {
   
   // TODO: add some means of signaling background thread to exit 
    // Loop 
    for(;;) {
        int rc1 = 0;
        int rc2 = 0;
        bool inc_cur_frame = false;

        VideoFrame_t& frame_1 = (*_frame_buffer)[_cur_frame_i];
        VideoFrame_t& frame_2 = (*_frame_buffer)[_cur_frame_i+1];

        if( (rc1 = pthread_rwlock_rdlock(frame_1.rw_lock)) != 0) {
            perror("Unable to acquire read lock in BgdCapturerSingle");
        }
        
        if( (rc2 = pthread_rwlock_rdlock(frame_2.rw_lock)) != 0) {
            perror("Unable to acquire 2nd read lock in BgdCapturerSingle");
        }

        if(difftime(frame_1.timestamp, // end
                    frame_2.timestamp)
                > 0) { // beg
            inc_cur_frame = true;
            std::cout << "inc" << std::endl;
        }

        if( (rc1 = pthread_rwlock_unlock(frame_1.rw_lock)) != 0) {
            perror("Unable to release read lock in BgdCapturerSingle");
        } 
        
        if( (rc2 = pthread_rwlock_unlock(frame_2.rw_lock)) != 0) {
            perror("Unable to release read lock in BgdCapturerSingle");
        }

       if (inc_cur_frame) {
           _cur_frame_i++;
       }
    }

    return false;
}

// TODO: implement
bool BgdCapturerSingle::getBgd(cv::Mat* bgd_buffer) {
    std::cout << "getbgd not implemented" << std::endl;
    return false;
}
