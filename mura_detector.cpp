//
// Created by tekatod on 3/21/18.
//

#include <cv.hpp>
#include "mura_detector.h"
#include "cv_tools.h"

void mura_detector::enable_debug() {
    if(!m_debug) {
        m_debug = true;
        cv::namedWindow("clustered");
        cv::namedWindow("thresh");
        cv::namedWindow("thresh2");
        cv::namedWindow("gray");
        cv::namedWindow("morphology");
        cv::namedWindow("found");
    }
}

size_t mura_detector::detect_mura(const cv::Mat& image) {
    cv::Mat processed = cv_tv::cluster(image, 3);
    if(m_debug) this->show("clustered", processed);
    cv::threshold(processed, processed, 45, 255, CV_THRESH_BINARY);
    if(m_debug) this->show("thresh", processed);
    cv::cvtColor(processed, processed, CV_BGR2GRAY);
    if(m_debug) this->show("gray", processed);
    cv::threshold(processed, processed, 100, 255, CV_THRESH_BINARY);
    if(m_debug) this->show("thresh2", processed);

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
        cv::Scalar color(0, 0, 0);
        cv::drawContours(c_img, countours, -1, color, 2);
        this->show("found", c_img);
    }
    return countours.size();
}
