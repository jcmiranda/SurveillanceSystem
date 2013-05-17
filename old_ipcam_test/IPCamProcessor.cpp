#include "IPCamProcessor.h"

bool IPCamProcessor::runInThread() {

    int rc = 0;
    for(;;) {
        
        int i = 0;
        i = system("wget -q http://192.168.2.30/cgi-bin/viewer/video.jpg");
        cv::Mat fromIPCam = cv::imread("video.jpg");
       
        if( (rc = pthread_rwlock_wrlock(&_frame_lock)) > 0) {
            perror("Unable to acquire write lock in ip cam processor");
            return false;
        }
        
        cv::resize(fromIPCam, 
                _frame,
                _frame.size());
        cvtColor(_frame, _frame, CV_BGR2GRAY);
        
        if( (rc = pthread_rwlock_unlock(&_frame_lock)) > 0) {
            perror("Unable to release write lock in ip cam processor");
            return false;
        }
        
        i = system("echo \"1\" >> log.txt");
        i = system("rm video.jpg");
    }
    return true;
}

bool IPCamProcessor::getFrame(cv::Mat* frame_buffer) {
    int rc = 0;
    if ( (rc = pthread_rwlock_rdlock(&_frame_lock)) != 0) {
        perror("unable to obtain read lock in getFrame");
    }

    _frame.copyTo(*frame_buffer);

    if ( (rc = pthread_rwlock_unlock(&_frame_lock)) != 0) {
        perror("unable to release read lock in getFrame");
    }
    return true;

}
