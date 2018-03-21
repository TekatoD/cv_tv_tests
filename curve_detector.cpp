//
// Created by tekatod on 3/21/18.
//

#include <cv.hpp>
#include <iostream>
#include "curve_detector.h"

void curve_detector::enable_debug() {
    if(!m_debug) {
        m_debug = true;
        cv::namedWindow("morphology");
        cv::namedWindow("countours");
        cv::namedWindow("curves");
        cv::namedWindow("gray");
        cv::namedWindow("canny");
        cv::namedWindow("sobel");
    }
}

size_t curve_detector::detect_curve(const cv::Mat& image) {
    cv::Mat processed;
    cv::cvtColor(image, processed, CV_BGR2GRAY);
    if(m_debug) this->show("gray", processed);
    cv::Canny(processed, processed, 20, 60,3);
    if(m_debug) this->show("canny", processed);
    std::vector<std::vector<cv::Point>> countours;
    std::vector<std::vector<cv::Point>> curves;

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(processed, processed, cv::MORPH_CLOSE, element);
    cv::threshold(processed, processed, 45, 255, CV_THRESH_BINARY);
    processed.convertTo(processed, CV_8UC1);
    if(m_debug) this->show("morphology", processed);

    cv::findContours(processed, countours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    if(m_debug) {
        cv::Mat c_img;
        image.copyTo(c_img);
        cv::Scalar color(0, 0, 255);
        cv::drawContours(c_img, countours, -1, color, 2);
        this->show("countours", c_img);
    }

    //TODO: opimize
    for(auto&& c : countours) {
        if(this->derivative(c, true) > 0.5) {
            curves.push_back(c);
        }
    }

    if(m_debug) {
        cv::Mat c_img;
        image.copyTo(c_img);
        cv::Scalar color(0, 0, 0);
        cv::drawContours(c_img, curves, -1, color, 2);
        this->show("curves", c_img);
    }


//    cv::threshold(processed, processed, 100, 255, CV_THRESH_BINARY);
//    cv::GaussianBlur(processed, processed, cv::Size{25, 25}, 0);
    if(m_debug) this->show("blurred", processed);
//    if(m_debug) this->show("thresh", processed);
    return 0;
}

double curve_detector::derivative(const std::vector<cv::Point>& contour, bool x) {
    double d = 0;
    if(x) {
        for(size_t i = 0; i < contour.size() - 1; ++i) {
            d += abs(contour[i + 1].x - contour[i].x);
        }
    } else {
        for(size_t i = 0; i < contour.size() - 1; ++i) {
            d += abs(contour[i + 1].y - contour[i].y);
        }
    }
    if(!contour.empty()) {
        d /= contour.size();
    }
    return d;
}
