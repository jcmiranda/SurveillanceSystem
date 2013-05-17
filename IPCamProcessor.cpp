#include "IPCamProcessor.h"

#include <vector>
#include <opencv2/opencv.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

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
    { if( matches[i].distance < 2.5*min_dist )
        { good_matches.push_back( matches[i]); }
    }

    //-- Draw only "good" matches
    Mat img_matches;
    drawMatches( img_1, keypoints_1, img_2, keypoints_2,
            good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
            vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    cvtColor(img_matches, img_matches, CV_BGR2GRAY);
  
   /* 
    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;

    for( int i = 0; i < good_matches.size(); i++ ) {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_1[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_2[ good_matches[i].trainIdx ].pt );
    }

    if (good_matches.size() > 3) {
        _H = findHomography( obj, scene, CV_RANSAC );    
    } */

    // getting access to motion blobs in right location 
    cvb::CvBlobs motion_blobs; 
    _motion_loc_blob_thresh->getLastMotionBlobs(&motion_blobs);

    int minx = 0;
    int miny = 0;
    int maxx = 0;
    int maxy = 0;

    for (cvb::CvBlobs::const_iterator it=motion_blobs.begin(); it!=motion_blobs.end(); ++it)
    {
        //std::cout << "Blob #" << it->second->label << 
        //    ": Area=" << it->second->area << std::endl;
        minx = (int) it->second->minx;
        miny = (int) it->second->miny;
        maxx = (int) it->second->maxx;
        maxy = (int) it->second->maxy;
        
        int ip_centerx = 0;
        int ip_centery = 0;

        int count = 0; 
       
       // Iterate over good matches and take the locations of the ones 
       // look up their location in the ip camera frame 
        for (int i = 0; i < good_matches.size(); i++) {
            int img_idx_1 = good_matches[i].queryIdx;
            cv::Point frame_pt =  keypoints_1[img_idx_1].pt;
       
            if(frame_pt.x > minx && frame_pt.x < maxx &&
                    frame_pt.y > miny && frame_pt.y < maxy) {
                count++;
                int img_idx_2 = good_matches[i].trainIdx;
                cv::Point ip_frame_pt =  keypoints_2[img_idx_2].pt;

                ip_centerx += ip_frame_pt.x;
                ip_centery += ip_frame_pt.y;
            }
        }
        if (count > 0)  {
            ip_centerx = ip_centerx/count;
            ip_centery = ip_centery/count;
            if (ip_centerx > _ip_center_x) 
                _ip_center_x = min(_ip_center_x + _ip_center_step, ip_centerx);
            else
                _ip_center_x = max(_ip_center_x - _ip_center_step, ip_centerx);
            
            if (ip_centery > _ip_center_y) 
                _ip_center_y = min(_ip_center_y + _ip_center_step, ip_centery);
            else
                _ip_center_y = max(_ip_center_y - _ip_center_step, ip_centery);
        }

// http://192.168.2.30/cgi-bin/camctrl/camctrl.cgi?&move=home
        // Want to move camera to re center
        if (abs(_ip_center_x - _frame_width/2) > _ip_radius) {
            cURLpp::Easy myRequest;
            std::stringstream result;
            if(_ip_center_x - _frame_width/2 < 0) {
                myRequest.setOpt(
                        cURLpp::Options::Url("http://192.168.2.30/cgi-bin/camctrl/camctrl.cgi?move=left"));
                std::cout << "left" << std::endl;
            } else {
                myRequest.setOpt(cURLpp::Options::Url("http://192.168.2.30/cgi-bin/camctrl/camctrl.cgi?move=right"));
                std::cout << "right" << std::endl;
            }
            myRequest.setOpt(cURLpp::Options::WriteStream(&result));
            myRequest.perform();                    

        } else if (abs(_ip_center_y - _frame_height/2) > _ip_radius) {
            cURLpp::Easy myRequest;
            std::stringstream result;
            if(_ip_center_y - _frame_height/2 < 0) {
                myRequest.setOpt(cURLpp::Options::Url("http://192.168.2.30/cgi-bin/camctrl/camctrl.cgi?move=up"));
                std::cout << "up" << std::endl;
            } else {
                myRequest.setOpt(cURLpp::Options::Url("http://192.168.2.30/cgi-bin/camctrl/camctrl.cgi?move=down"));
                std::cout << "down" << std::endl;
            }
            myRequest.setOpt(cURLpp::Options::WriteStream(&result));
            myRequest.perform();                    

        }
        
        cv::circle(img_matches, cv::Point(_ip_center_x + _frame_width,
                    _ip_center_y), 5, 255, 2, 8);
        
        cv::rectangle(img_matches, cv::Point(minx, miny),
                  cv::Point(maxx, maxy),
              255,
            3,
          8);  

    } 

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

// IP Camera Homography code
                /*
                if (ip_frame_pt.x < ip_minx) { 
                    ip_minx = ip_frame_pt.x;
                }
                if (ip_frame_pt.x > ip_maxx) { 
                    ip_maxx = ip_frame_pt.x;
                }
                if (ip_frame_pt.y < ip_miny) { 
                    ip_miny = ip_frame_pt.y;
                }
                if (ip_frame_pt.y > ip_maxy) { 
                    ip_maxy = ip_frame_pt.y;
                } */
       // int ip_minx = 1000;
        //int ip_miny = 1000;
        //int ip_maxx = 0;
        //int ip_maxy = 0;


        /*
        //-- Get the corners from the image_1 ( the object to be "detected" )
        std::vector<Point2f> obj_corners(4);
        obj_corners[0] = cv::Point(minx, miny); // cvPoint(0,0); 
        obj_corners[1] = cv::Point(maxx, miny); // cvPoint( img_object.cols, 0 );
        obj_corners[2] = cv::Point(maxx, maxy); // cvPoint( img_object.cols, img_object.rows ); 
        obj_corners[3] = cv::Point(minx, maxy); // cvPoint( 0, img_object.rows );
        
        std::vector<Point2f> scene_corners(4);

        perspectiveTransform( obj_corners, scene_corners, _H);

        cv::Point2f offset = cv::Point2f(_frame_width, 0);
        //-- Draw lines between the corners (the mapped object in the scene - image_2 )
        line( img_matches, 
                scene_corners[0]  + offset, //+ Point2f( img_object.cols, 0), 
                scene_corners[1] + offset, // Point2f( img_object.cols, 0), 
                Scalar(0, 255, 0), 4 );
        line( img_matches, 
                scene_corners[1] + offset, // img_object.cols, 0), 
                scene_corners[2] + offset, // Point2f(_img_width, 0); // Point2f( img_object.cols, 0), 
                Scalar( 0, 255, 0), 4 );
        line( img_matches, 
                scene_corners[2] + offset, // Point2f( img_object.cols, 0), 
                scene_corners[3] + offset, // Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
                Scalar( 0, 255, 0), 4 );
        line( img_matches, 
                scene_corners[3] + offset, // Point2f( img_object.cols, 0), 
                scene_corners[0] + offset, 
                Scalar( 0, 255, 0), 4 ); */
//
