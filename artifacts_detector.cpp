//
// Created by tekatod on 3/20/18.
//

#include <cv.hpp>
#include "artifacts_detector.h"

void artifacts_detector::disable_debug() {
    if(m_debug) {
        m_debug = false;
        cv::destroyAllWindows();
    }
}

void artifacts_detector::enable_debug() {
    if(!m_debug) {
        m_debug = true;
        cv::namedWindow("gray");
        cv::namedWindow("blurred");
        cv::namedWindow("magnitude");
        cv::namedWindow("morphology");
        cv::namedWindow("found");
    }
}

void artifacts_detector::show(const std::string& window_name, const cv::Mat& img) {
    cv::imshow(window_name, img);
    cv::waitKey(1);
}

size_t artifacts_detector::detect_artifacts(const cv::Mat &image) {
    cv::Mat processed = this->cluster(image, 2);
    cv::cvtColor(processed, processed, CV_BGR2GRAY);
    if(m_debug) this->show("gray", processed);
    cv::GaussianBlur(processed, processed, cv::Size{15, 15}, 0);
    if(m_debug) this->show("blurred", processed);
    cv::Mat dx, dy;
    Sobel(processed, dx, CV_32F, 1, 0);
    Sobel(processed, dy, CV_32F, 0, 1);
    cv::magnitude(dx, dy, processed);
    if(m_debug) this->show("magnitude", processed);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(processed, processed, cv::MORPH_CLOSE, element);
    if(m_debug) this->show("morphology", processed);
    cv::threshold(processed, processed, 45, 255, CV_THRESH_BINARY);
    processed.convertTo(processed, CV_8UC1);
    std::vector<std::vector<cv::Point>> countours;
    cv::findContours(processed, countours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//    for(size_t i = 0; i < countours.size(); ++i) {
//        double p = cv::arcLength(countours[i], true);
//        cv::approxPolyDP(countours[i], countours[i], 0.1 * p, true);
//    }
    if(m_debug) {
        cv::Mat c_img;
        image.copyTo(c_img);
        c_img.convertTo(c_img, CV_8UC3);
        cv::Scalar color(0, 0, 255);
        cv::drawContours(c_img, countours, -1, color, 2);
        this->show("found", c_img);
    }
    return countours.size();
}

cv::Mat artifacts_detector::cluster(const cv::Mat &image, size_t cluster_count) {
    cv::Mat samples(image.rows * image.cols, 3, CV_32F);
    for( int y = 0; y < image.rows; y++ ) {
        for (int x = 0; x < image.cols; x++) {
            for (int z = 0; z < 3; z++) {
                samples.at<float>(y + x * image.rows, z) = image.at<cv::Vec3b>(y, x)[z];
            }
        }
    }


    cv::Mat labels;
    int attempts = 5;
    cv::Mat centers;
    kmeans(samples, cluster_count, labels, cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts,
           cv::KMEANS_PP_CENTERS, centers );

    cv::Mat new_image( image.size(), image.type() );
    for( int y = 0; y < image.rows; y++ )
        for( int x = 0; x < image.cols; x++ )
        {
            int cluster_idx = labels.at<int>(y + x*image.rows,0);
            new_image.at<cv::Vec3b>(y,x)[0] = centers.at<float>(cluster_idx, 0);
            new_image.at<cv::Vec3b>(y,x)[1] = centers.at<float>(cluster_idx, 1);
            new_image.at<cv::Vec3b>(y,x)[2] = centers.at<float>(cluster_idx, 2);
        }
    return std::move(new_image);
}
