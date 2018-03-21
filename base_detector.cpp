//
// Created by tekatod on 3/21/18.
//

#include <cv.hpp>
#include "base_detector.h"

void base_detector::disable_debug() {
    if(m_debug) {
        m_debug = false;
        cv::destroyAllWindows();
    }
}

void base_detector::show(const std::string& window_name, const cv::Mat& img) {
    cv::imshow(window_name, img);
    cv::waitKey(1);
}
