//
// Created by tekatod on 3/21/18.
//

#pragma once

#include <cv.hpp>


namespace cv_tv {

    cv::Mat cluster(const cv::Mat& image, size_t cluster_count);
}