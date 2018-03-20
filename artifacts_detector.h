//
// Created by tekatod on 3/20/18.
//

#pragma once

#include <opencv2/core/mat.hpp>

class artifacts_detector {
public:
    artifacts_detector() = default;

    void enable_debug();

    void disable_debug();

    void detect_artifacts(const cv::Mat& image);


private:

    void show(const std::string& window_name, const cv::Mat& img);

    cv::Mat cluster(const cv::Mat &image, size_t cluster_count);

private:

    bool m_debug{false};
};