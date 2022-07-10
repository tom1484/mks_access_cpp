#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <unistd.h>
#include <string>
#include <ctime>
#include <iostream>


class Frame {

public:

    void initialize(const int _fb, const int _width, const int _height);

    void showCurrentTime(std::string currTime);

private:

    std::ofstream ofs;

    int width, height;

};