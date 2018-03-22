cmake_minimum_required(VERSION 3.9)
project(cv_exp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(cv_exp main.cpp
        rect_detector.cpp
        rect_detector.h
        artifacts_detector.cpp
        artifacts_detector.h
        cv_tools.h
        base_detector.cpp
        base_detector.h
        mura_detector.cpp
        mura_detector.h
        cv_tools.cpp
        curve_detector.cpp
        curve_detector.h)

target_link_libraries(cv_exp ${OpenCV_LIBRARIES})