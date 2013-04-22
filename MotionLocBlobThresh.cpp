#include "MotionLocBlobThresh.h"

#include <vector>
#include <opencv2/opencv.hpp>

bool MotionLocBlobThresh::processFrame() {
    cv::Mat mask(_frame_height, _frame_width, CV_8UC1,
            cv::Scalar(0));
    cv::Mat bgd(_frame_height, _frame_width, CV_8UC1,
            cv::Scalar(0));
    getBgd(&bgd);
    VideoFrame_t& this_frame = (*_frame_buffer)[_cur_frame_i];
    
    // Calculate motion probabilities
    _motion_prob_y_diff.getMotionProbs(this_frame.frame, 
            bgd, 
            &mask);
   
    int rc = 0; 
    if( (rc = pthread_rwlock_wrlock(&_last_prob_mask_lock)) != 0) {
        perror("unable to lock on last prob mask.");
    }
    if( (rc = pthread_rwlock_wrlock(&_motion_blobs_lock)) != 0) {
        perror("unable to lock on motion blobs mask.");
    }

    mask.copyTo(_last_prob_mask);

    cv::Mat thresh_mask;
    cv::threshold(mask, thresh_mask, 6, 256, 0);    

    int morph_size = 4;
    cv::Mat element = cv::getStructuringElement(2, // ellipse
            cv::Size(2 * morph_size + 1, 
                2 * morph_size + 1), 
            cv::Point( morph_size, morph_size));
    cv::morphologyEx(thresh_mask, 
            thresh_mask, 
            2, //opening
            element);
    cv::morphologyEx(thresh_mask, 
            thresh_mask, 
            3,  // Closing
            element);
 

    IplImage* thresh_ipl = new IplImage(thresh_mask);
    cvLabel(thresh_ipl, 
            _label_img, 
            _motion_blobs);

    if( (rc = pthread_rwlock_unlock(&_last_prob_mask_lock)) != 0) {
        perror("unable to unlock on last prob mask.");
    }
    if( (rc = pthread_rwlock_unlock(&_motion_blobs_lock)) != 0) {
        perror("unable to unlock on motion blobs mask.");
    }
    return true;
}


bool MotionLocBlobThresh::getLastProbMask(cv::Mat* dst) {
    int rc = 0;
    if( (rc = pthread_rwlock_rdlock(&_last_prob_mask_lock)) != 0) {
        perror("unable to lock on prob mask.");
    }

    _last_prob_mask.copyTo(*dst);

    if( (rc = pthread_rwlock_unlock(&_last_prob_mask_lock)) != 0) {
        perror("unable to unlock on prob mask.");
    }
    return true;
}

bool MotionLocBlobThresh::getLastMotionBlobs(cvb::CvBlobs* blobs) {
    int rc = 0;
    if( (rc = pthread_rwlock_rdlock(&_motion_blobs_lock)) != 0) {
        perror("unable to lock on motion blobs.");
    }

    *blobs = _motion_blobs;

    if( (rc = pthread_rwlock_unlock(&_motion_blobs_lock)) != 0) {
        perror("unable to unlock on motion blobs.");
    }
    return true;
}

// mat should be CV_U8C3 matrix
bool MotionLocBlobThresh::annotateMatWithBlobs(cv::Mat* mat) {
    int rc = 0;
    if( (rc = pthread_rwlock_rdlock(&_motion_blobs_lock)) != 0) {
        perror("unable to lock on motion blobs.");
    }
    
    IplImage* blob_frame = new IplImage(*mat);
    cvb::cvRenderBlobs(_label_img, 
            _motion_blobs, 
            blob_frame, 
            blob_frame,
            CV_BLOB_RENDER_COLOR,
            0.5);
    cvb::cvRenderBlobs(_label_img, 
            _motion_blobs, 
            blob_frame, 
            blob_frame,
            CV_BLOB_RENDER_BOUNDING_BOX);

    *mat = blob_frame;
    
    if( (rc = pthread_rwlock_unlock(&_motion_blobs_lock)) != 0) {
        perror("unable to unlock on motion blobs.");
    }
    return true;
}
