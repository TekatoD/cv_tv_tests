//
// Created by tekatod on 3/21/18.
//

#pragma once


#include "base_detector.h"

class mura_detector : public base_detector {
public:

    mura_detector() = default;

    void enable_debug() override;

    size_t detect_mura(const cv::Mat& image);

};
