#include "IPCamProcessor.h"

#include <vector>
#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "cvblob.h"

using namespace std;
using namespace cv;

// When process frame is called, this thread holds rd locks on
// _cur_frame_i and _cur_frame_i + 1
bool IPCamProcessor::processFrame() {
    cv::Mat img_1 = (*_frame_buffer)[_cur_frame_i].color_frame;
    cv::Mat img_2 = (*_frame_buffer)[_cur_frame_i].color_ip_frame;

    // annotate pair with feature point matches and convert to
    // grayscale
    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 400;

    SurfFeatureDetector detector( minHessian );

    std::vector<KeyPoint> keypoints_1, keypoints_2;

    detector.detect( img_1, keypoints_1 );
    detector.detect( img_2, keypoints_2 );

    //-- Step 2: Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;

    Mat descriptors_1, descriptors_2;

    extractor.compute( img_1, keypoints_1, descriptors_1 );
    extractor.compute( img_2, keypoints_2, descriptors_2 );

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_1, descriptors_2, matches );

    double max_dist = 0; double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_1.rows; i++ )
    { double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }

    // printf("-- Max dist : %f \n", max_dist );
    // printf("-- Min dist : %f \n", min_dist );

    //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
    //-- PS.- radiusMatch can also be used here.
    std::vector< DMatch > good_matches;

    for( int i = 0; i < descriptors_1.rows; i++ )
    { if( matches[i].distance < 2*min_dist )
        { good_matches.push_back( matches[i]); }
    }

    //-- Draw only "good" matches
    Mat img_matches;
    drawMatches( img_1, keypoints_1, img_2, keypoints_2,
            good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
            vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    cvtColor(img_matches, img_matches, CV_BGR2GRAY);
 
    // getting access to motion blobs in right location 
    cvb::CvBlobs motion_blobs; 
    _motion_loc_blob_thresh->getLastMotionBlobs(&motion_blobs);

    int rc = 0;
    if( (rc = pthread_rwlock_wrlock(&_last_pair_lock)) != 0) {
        perror("unable to lock on last pair.");
    }

    img_matches.copyTo(_last_pair);

    if( (rc = pthread_rwlock_unlock(&_last_pair_lock)) != 0) {
        perror("unable to unlock on last pair.");
    }
    return true;
}

bool IPCamProcessor::getLastPair(cv::Mat* dst) {
    int rc = 0;
    if( (rc = pthread_rwlock_rdlock(&_last_pair_lock)) != 0) {
        perror("unable to lock on last pair.");
    }

    _last_pair.copyTo(*dst);

    if( (rc = pthread_rwlock_unlock(&_last_pair_lock)) != 0) {
        perror("unable to unlock on last pair.");
    }
    return true;
}
