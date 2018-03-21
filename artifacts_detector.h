//
// Created by tekatod on 3/20/18.
//

#pragma once

#include <opencv2/core/mat.hpp>
#include "base_detector.h"

class artifacts_detector : public base_detector {
public:
    artifacts_detector() = default;

    void enable_debug() override ;

    size_t detect_artifacts(const cv::Mat &image);
};