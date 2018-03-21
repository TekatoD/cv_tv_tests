//
// Created by tekatod on 3/19/18.
//

#pragma once


#include <opencv2/core/types.hpp>
#include "base_detector.h"

class rect_detector : public base_detector {
public:
    rect_detector() = default;

    cv::Mat detect_screen(const cv::Mat &image);

    void enable_debug() override ;

private:
    std::vector<std::vector<cv::Point>> find_contours(const cv::Mat &image);
};
