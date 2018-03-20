#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <cv.hpp>
#include "rect_detector.h"
#include "artifacts_detector.h"

int main() {
    rect_detector detector;
    artifacts_detector artifacts_detector1;
    cv::namedWindow("original");
    cv::Mat img = cv::imread("/home/tekatod/Pictures/IMG_1104.JPG");
    cv::imshow("original", img);
    cv::waitKey(1);
    artifacts_detector1.enable_debug();
//    detector.enable_debug();
    cv::Mat scr = detector.detect_screen(img);
    artifacts_detector1.detect_artifacts(scr);
    cv::imwrite("screen.jpg", scr);
    cv::waitKey();
}