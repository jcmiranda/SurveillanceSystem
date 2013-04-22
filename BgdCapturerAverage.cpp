#include "BgdCapturerAverage.h"
#include "video_frame.h"

// When process frame is called, this thread holdes rd locks on
// _cur_frame_i and _cur_frame_i + 1
bool BgdCapturerAverage::processFrame() {
    _ctr = (_ctr + 1) % _step;
    // Occurs every _step frames
    if (_ctr == 0) {
        addFrameToBgd();
        updateBgd();
    }
    return true;
}

bool BgdCapturerAverage::updateBgd() {
    // Once per time the bgd frame buffer is filled, update
    // the bgd frame. Avoids problem of having half filled
    // buffer for bgd frames first time through which would mess 
    // with averages.
    if (_bgd_frame_i == (_frames_per_bgd - 1)) {
        cv::Mat bgd_float_sum = cv::Mat(_frame_height, 
                _frame_width, CV_32FC1, cv::Scalar(0));
        
        int start_iter = _frames_for_bgd.size() - 1;
        for (int i = start_iter; i >= 0; i--) {
            cv::Mat bgd_float_single = cv::Mat(_frame_height,
                    _frame_width, CV_32FC1, cv::Scalar(0));

            _frames_for_bgd[i].convertTo(bgd_float_single, CV_32FC1);

            cv::add(bgd_float_sum, bgd_float_single, bgd_float_sum);
        }

        cv::Mat bgd_8uc1(_frame_height, _frame_width, CV_8UC1, 
                cv::Scalar(0));
        bgd_float_sum.convertTo(bgd_8uc1, CV_8UC1, 
                1.0 / (1.0 * _frames_per_bgd));

        setBgd(bgd_8uc1);
    }
    return true;
}

bool BgdCapturerAverage::addFrameToBgd() {
    VideoFrame_t& this_frame = (*_frame_buffer)[_cur_frame_i];
    
    // Copy frame to buffer of bgd frames 
    (this_frame.frame).copyTo(_frames_for_bgd[_bgd_frame_i]);
    // Increment index of where to write into bgd buffer
    _bgd_frame_i = (_bgd_frame_i + 1) % _frames_per_bgd;
    return true;
}

/*
        // compare to _cur_frame_i
        cv::Mat frame_diff;
        cv::absdiff(this_frame.frame, bgd_8uc1, frame_diff);
        unsigned char max_diff = *std::max_element(
                frame_diff.begin<unsigned char>(), 
                frame_diff.end<unsigned char>());
        unsigned char min_frame = *std::min_element(
                (this_frame.frame).begin<unsigned char>(), 
                (this_frame.frame).end<unsigned char>());
        unsigned char min_bgd = *std::min_element(
                bgd_8uc1.begin<unsigned char>(), 
                bgd_8uc1.end<unsigned char>());
        printf("Diff most recent frame - bgd: 0x%02x\n", max_diff);
        printf("Min most recent frame : 0x%02x\n", min_frame);
        printf("Min bgd: 0x%02x\n", min_bgd);
 */
