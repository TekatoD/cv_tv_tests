//
// Created by tekatod on 3/20/18.
//

#include <cv.hpp>
#include "artifacts_detector.h"
#include "cv_tools.h"

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


size_t artifacts_detector::detect_artifacts(const cv::Mat &image) {
    cv::Mat processed = cv_tv::cluster(image, 2);
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
    cv::threshold(processed, processed, 10, 255, CV_THRESH_BINARY);
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
        cv::Scalar color(0, 255, 0);
        cv::drawContours(c_img, countours, -1, color, 2);
        this->show("found", c_img);
    }
    return countours.size();
}