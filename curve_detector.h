//
// Created by tekatod on 3/21/18.
//

#pragma once

#include "base_detector.h"

class curve_detector : public base_detector {
public:

    curve_detector() = default;

    void enable_debug() override;

    size_t detect_curve(const cv::Mat& image);

private:

    double derivative(const std::vector<cv::Point>& contour, bool x);

};
