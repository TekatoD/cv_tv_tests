//
// Created by tekatod on 3/21/18.
//

#include "cv_tools.h"

cv::Mat cv_tv::cluster(const cv::Mat& image, size_t cluster_count) {
    cv::Mat samples(image.rows * image.cols, 3, CV_32F);
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int z = 0; z < 3; z++) {
                samples.at<float>(y + x * image.rows, z) = image.at<cv::Vec3b>(y, x)[z];
            }
        }
    }


    cv::Mat labels;
    int attempts = 5;
    cv::Mat centers;
    cv::kmeans(samples, cluster_count, labels, cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001),
               attempts,
               cv::KMEANS_PP_CENTERS, centers);

    cv::Mat new_image(image.size(), image.type());
    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++) {
            int cluster_idx = labels.at<int>(y + x * image.rows, 0);
            new_image.at<cv::Vec3b>(y, x)[0] = centers.at<float>(cluster_idx, 0);
            new_image.at<cv::Vec3b>(y, x)[1] = centers.at<float>(cluster_idx, 1);
            new_image.at<cv::Vec3b>(y, x)[2] = centers.at<float>(cluster_idx, 2);
        }
    return std::move(new_image);
}

