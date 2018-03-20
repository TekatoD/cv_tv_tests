//
// Created by tekatod on 3/19/18.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <cv.hpp>
#include <iostream>
#include "rect_detector.h"

std::vector<std::vector<cv::Point>> rect_detector::find_contours(const cv::Mat& image) {
    cv::Mat processed;
    cv::cvtColor(image, processed, CV_BGR2GRAY);
    if(m_debug) this->show("gray", processed);
    cv::GaussianBlur(processed, processed, cv::Size{25, 25}, 0);
    if(m_debug) this->show("blurred", processed);
    cv::threshold(processed, processed, 45, 255, CV_THRESH_BINARY);
    if(m_debug) this->show("thresh", processed);
    std::vector<std::vector<cv::Point>> countours;
    cv::findContours(processed, countours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    if(m_debug) {
        cv::Mat c_img;
        image.copyTo(c_img);
        cv::Scalar color(0, 0, 255);
        cv::drawContours(c_img, countours, -1, color, 2);
        for(auto&& c : countours) {
            cv::Moments m = cv::moments(c);
            int c_x = m.m10 / m.m00;
            int c_y = m.m01 / m.m00;
            cv::circle(c_img, cv::Point(c_x, c_y), 7, color, 2);
        }
        this->show("countours", c_img);
    }
    return std::move(countours);
}

void rect_detector::disable_debug() {
    if(m_debug) {
        m_debug = false;
        cv::destroyAllWindows();
    }
}

void rect_detector::enable_debug() {
    if(!m_debug) {
        m_debug = true;
        cv::namedWindow("gray");
        cv::namedWindow("blurred");
        cv::namedWindow("thresh");
        cv::namedWindow("rect");
        cv::namedWindow("rotated");
    }
}

void rect_detector::show(const std::string& window_name, const cv::Mat& img) {
    cv::imshow(window_name, img);
    cv::waitKey(1);
}

cv::Mat rect_detector::detect_screen(const cv::Mat &image) {
    auto countours = this->find_contours(image);
    auto max_per = std::max_element(countours.begin(), countours.end(), [](auto&& c1, auto&& c2) {
        return cv::arcLength(c1, true) < cv::arcLength(c2, true);
    });
    std::vector<cv::Point> aprox_countor;
    double p = cv::arcLength(*max_per, true);
    cv::approxPolyDP(*max_per, aprox_countor, 0.1 * p, true);
    cv::Rect tv = cv::boundingRect(aprox_countor);
    if(m_debug) {
        cv::Mat c_img;
        image.copyTo(c_img);
        cv::Scalar color(0, 0, 255);
        cv::drawContours(c_img, std::vector<std::vector<cv::Point>>{{aprox_countor}}, -1, color, 2);
        this->show("rect", c_img);
    }
    if(aprox_countor.size() == 4) {
        cv::Point2f src_vertices[4];
        cv::Point2f dst_vertices[4];
        std::sort(aprox_countor.begin(), aprox_countor.end(), [](auto&& p1, auto&& p2){
            return p1.y < p2.y;
        });
        if(aprox_countor[0].x > aprox_countor[1].x) {
            std::swap(aprox_countor[0], aprox_countor[1]);
        }
        if(aprox_countor[2].x > aprox_countor[3].x) {
            std::swap(aprox_countor[2], aprox_countor[3]);
        }
        for(size_t i = 0; i < 4; ++i) {
            src_vertices[i] = aprox_countor[i];
        }
//        src_vertices[0] = aprox_countor[0];
//        src_vertices[1] = aprox_countor[3];
//        src_vertices[2] = aprox_countor[1];
//        src_vertices[3] = aprox_countor[2];
        dst_vertices[0] = cv::Point(0, 0);
        dst_vertices[1] = cv::Point(tv.width - 1, 0);
        dst_vertices[2] = cv::Point(0, tv.height - 1);
        dst_vertices[3] = cv::Point(tv.width - 1, tv.height - 1);
        cv::Mat perspective = cv::getPerspectiveTransform(src_vertices, dst_vertices);
        cv::Mat rotated;
        cv::Size size(tv.width, tv.height);
        cv::warpPerspective(image, rotated, perspective, size, CV_INTER_LINEAR, cv::BORDER_CONSTANT);
        if(m_debug) this->show("rotated", rotated);
        return std::move(rotated);
    }
    return cv::Mat();
}
