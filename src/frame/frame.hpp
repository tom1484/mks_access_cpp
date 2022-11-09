#ifndef FRAME_HPP_
#define FRAME_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/freetype.hpp>
#include <fstream>
#include <unistd.h>
#include <string>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>

namespace Frame
{

    void initialize(int _fb, int _width, int _height);

    void showLine(std::string textVec, cv::Scalar fontColor, cv::Scalar bgColor, double scale, int thickness);
    void showLines(std::vector<std::string> text, cv::Scalar fontColor, cv::Scalar bgColor, double fontHeight, int thickness);

    void lock(int _lockPeriod);
    void unlock();
    void timer();
}

#endif