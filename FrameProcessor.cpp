#include "FrameProcessor.h"

bool FrameProcessor::runInThread() {

    // TODO: add some means of signaling thread to exit 
    // Loop 
    for(;;) {
        int rc1 = 0;
        int rc2 = 0;
        int rc3 = 0;
        bool inc_cur_frame = false;
        bool process_frame = false;

        do {
            VideoFrame_t& frame_1 = (*_frame_buffer)[_cur_frame_i];
            int next_i = (_cur_frame_i+1) % _buffer_length;
            int next_next_i = (_cur_frame_i+2) % _buffer_length;
            
            VideoFrame_t& frame_2 = 
                (*_frame_buffer)[next_i];
            
            VideoFrame_t& frame_3 = 
                (*_frame_buffer)[next_next_i];

            if( (rc1 = pthread_rwlock_rdlock(frame_1.rw_lock)) > 0) {
                perror("Unable to acquire read lock in FrameProcessor");
                return false;
            }

            if( (rc2 = pthread_rwlock_rdlock(frame_2.rw_lock)) > 0) {
                perror("Unable to acquire 2nd read lock in FrameProcessor");
                return false;
            }
            
            if( (rc3 = pthread_rwlock_rdlock(frame_3.rw_lock)) > 0) {
                perror("Unable to acquire 3rd read lock in FrameProcessor");
                return false;
            }

            // If thread should be exited, release appropriate read locks
            // and exit the thread
            if (frame_1.exit_thread || frame_2.exit_thread) {
                if( (rc1 = pthread_rwlock_unlock(frame_1.rw_lock)) > 0) {
                    perror("Unable to release read lock in BgdCapturerAverage");
                    return false;
                } 

                if( (rc2 = pthread_rwlock_unlock(frame_2.rw_lock)) > 0) {
                    perror("Unable to release read lock 2 in BgdCapturerAverage");
                    return false;
                }
                if( (rc3 = pthread_rwlock_unlock(frame_3.rw_lock)) > 0) {
                    perror("Unable to release read lock 3 in BgdCapturerAverage");
                    return false;
                }
                
                return true;

            }


            // New frame available
            if(difftime(frame_2.timestamp, // end
                        frame_1.timestamp) // beg
                    > 0 ) {
                
                inc_cur_frame = true;;   
            }
            if ( difftime( frame_2.timestamp, 
                 frame_3.timestamp) > 0) { 
                inc_cur_frame = true;
                process_frame = true;
                processFrame();
            }

            if( (rc1 = pthread_rwlock_unlock(frame_1.rw_lock)) > 0) {
                perror("Unable to release read lock in BgdCapturerAverage");
                return false;
            } 

            if( (rc2 = pthread_rwlock_unlock(frame_2.rw_lock)) > 0) {
                perror("Unable to release read lock 2 in BgdCapturerAverage");
                return false;
            }
            
            if( (rc3 = pthread_rwlock_unlock(frame_3.rw_lock)) > 0) {
                perror("Unable to release read lock 3 in BgdCapturerAverage");
                return false;
            }
            if (inc_cur_frame) {
                _cur_frame_i = (_cur_frame_i + 1) % _buffer_length;
            }
        } while(!process_frame);

    }

    return false;
}

// TODO: do all frame processors need a means to get a background
// only background frame processors. 
// Host is responsible for locking on bgd_dest
// TODO rework such that there is potentially one bgd frame shared 
// by all? not sure if this makes more sense?
bool FrameProcessor::getBgd(cv::Mat* bgd_dest) {
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
bool FrameProcessor::setBgd(const cv::Mat& bgd) {
    if(pthread_rwlock_wrlock(&_bgd_lock) != 0) {
        perror("could not obtain bgd write lock to set bgd");
        return false;
    } 

    bgd.copyTo(_bgd);
    std::cout << "New bgd set" << std::endl;

    if(pthread_rwlock_unlock(&_bgd_lock) != 0) {
        perror("could not release write lock to set bgd");
        return false;
    } 
    return true;
}

