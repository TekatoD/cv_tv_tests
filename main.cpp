#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <cv.hpp>
#include "rect_detector.h"
#include "artifacts_detector.h"
#include "mura_detector.h"

int main() {
    rect_detector detector;
    artifacts_detector artifacts_detector1;
    mura_detector mura_detector1;
    cv::namedWindow("original");
    cv::Mat img = cv::imread("/home/tekatod/develop/cv_tv_tests/some_test/tv3.jpg");
    cv::imshow("original", img);
    cv::waitKey(1);
    mura_detector1.enable_debug();
//    artifacts_detector1.enable_debug();
//    detector.enable_debug();
    cv::Mat scr = detector.detect_screen(img);
    artifacts_detector1.detect_artifacts(scr);
    mura_detector1.detect_mura(scr);
    cv::imwrite("screen.jpg", scr);
    cv::waitKey();
}