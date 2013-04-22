#ifndef BGD_CAPTURER_SINGLE_H
#define BGD_CAPTURER_SINGLE_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "FrameProcessor.h"
#include "video_frame.h"

class BgdCapturerAverage : public FrameProcessor {
    public:
        BgdCapturerAverage(std::vector<VideoFrame_t>* frame_buffer,
                int buffer_length, int frame_width, int frame_height,
                int frames_per_bgd) : 
            FrameProcessor(frame_buffer, buffer_length, 
                    frame_width, frame_height),
            _ctr(0), _step(120), _frames_per_bgd(frames_per_bgd) {
                _frames_for_bgd
                    = std::vector<cv::Mat>(_frames_per_bgd,
                            cv::Mat(frame_height, 
                                frame_width,
                                CV_8UC1,
                                cv::Scalar(0)));
            };
        virtual bool processFrame();
    private:
        int _ctr;
        int _step;
        std::vector<cv::Mat> _frames_for_bgd;
        int _frames_per_bgd;
};

#endif
