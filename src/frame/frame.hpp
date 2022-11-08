#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/freetype.hpp>
#include <fstream>
#include <unistd.h>
#include <string>
#include <ctime>
#include <iostream>


namespace Frame {

    void initialize(const int _fb, const int _width, const int _height);

    void showText(std::string textVec, cv::Scalar fontColor, cv::Scalar bgColor, double scale, int thickness);
    void showTextStyle(std::vector<std::string> text, cv::Scalar fontColor, cv::Scalar bgColor, double fontHeight, int thickness);

}
