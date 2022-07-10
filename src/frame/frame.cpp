#include "frame.hpp"

void Frame::initialize(const int _fb, const int _width, const int _height) {
    // set frame dimension
    width = _width;
    height = _height;

    // set framebuffer stream
    std::string fbDevice = "/dev/fb";
    fbDevice += std::to_string(_fb);
    ofs.open(fbDevice.c_str());
}

void Frame::showCurrentTime(std::string currTime) {
    
    int font_face = cv::FONT_HERSHEY_SIMPLEX;
    double font_scale = 4;
    int thickness = 5;
    int baseline;

    cv::Mat image = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
    cv::Mat framebuffer_compat;
    
    // get text size
    cv::Size text_size = cv::getTextSize(currTime, font_face, font_scale, thickness, &baseline);

    cv::Point origin;
    origin.x = image.cols / 2 - text_size.width / 2;
    origin.y = image.rows / 2 + text_size.height / 2;
    cv::putText(image, currTime, origin, font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);

    cv::cvtColor(image, framebuffer_compat, cv::COLOR_BGR2BGR565);
    for (int y = 0; y < height ; y++) {
        ofs.seekp(y * width * 2);
        ofs.write(reinterpret_cast<char*>(framebuffer_compat.ptr(y)), width * 2);
    }

}