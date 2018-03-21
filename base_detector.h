//
// Created by tekatod on 3/21/18.
//

#pragma once

#include <opencv2/core/mat.hpp>

class base_detector {
public:
    virtual void enable_debug() = 0;

    void disable_debug();

protected:

    void show(const std::string& window_name, const cv::Mat& img);

protected:

    bool m_debug{false};
};