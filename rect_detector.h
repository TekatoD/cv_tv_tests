//
// Created by tekatod on 3/19/18.
//

#pragma once


#include <opencv2/core/types.hpp>

class rect_detector {
public:
    rect_detector() = default;

    cv::Mat detect_screen(const cv::Mat &image);

    void enable_debug();

    void disable_debug();

private:
    std::vector<std::vector<cv::Point>> find_contours(const cv::Mat &image);

    void show(const std::string& window_name, const cv::Mat& img);

private:
    bool m_debug{false};
};
