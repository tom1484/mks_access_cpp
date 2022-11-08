#include "frame.hpp"

namespace Frame
{

    std::ofstream ofs;
    int width, height;

    void initialize(int _fb, int _width, int _height)
    {

        // set frame dimension
        width = _width;
        height = _height;

        // set framebuffer stream
        std::string fbDevice = "/dev/fb";
        fbDevice += std::to_string(_fb);
        ofs.open(fbDevice.c_str());
    }

    void showTextStyle(std::vector<std::string> textVec, cv::Scalar fontColor, cv::Scalar bgColor, double fontHeight, int thickness)
    {

        cv::Ptr<cv::freetype::FreeType2> ft2;
        ft2 = cv::freetype::createFreeType2();
        ft2->loadFontData("./FiraCode.ttf", 0);

        cv::Mat image = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
        cv::Mat framebuffer_compat;

        // set background
        image.setTo(bgColor);

        for (int i = 0; i < textVec.size(); i++)
        {
            std::string text = textVec[i];
            int baseline = 0;

            // get text size
            cv::Size text_size = ft2->getTextSize(text, fontHeight, thickness, &baseline);

            cv::Point origin;
            origin.x = image.cols / 2 - text_size.width / 2;
            origin.y = image.rows / 2 + text_size.height / 2 - int((text_size.height + 40) * (i - (textVec.size() - 1) / 2.0));
            // origin.y = image.rows / 2 + text_size.height / 2;
            ft2->putText(image, text, origin, fontHeight, fontColor, thickness, 8, true);
        }

        cv::cvtColor(image, framebuffer_compat, cv::COLOR_RGB2BGR565);
        for (int y = 0; y < height; y++)
        {
            ofs.seekp(y * width * 2);
            ofs.write(reinterpret_cast<char *>(framebuffer_compat.ptr(y)), width * 2);
        }
    }

    void showText(std::string text, cv::Scalar fontColor, cv::Scalar bgColor, double scale, int thickness)
    {

        int font_face = cv::FONT_HERSHEY_SIMPLEX;
        int baseline;

        cv::Mat image = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
        cv::Mat framebuffer_compat;

        // set background
        image.setTo(bgColor);

        // get text size
        cv::Size text_size = cv::getTextSize(text, font_face, scale, thickness, &baseline);

        cv::Point origin;
        origin.x = image.cols / 2 - text_size.width / 2;
        origin.y = image.rows / 2 + text_size.height / 2;
        cv::putText(image, text, origin, font_face, scale, fontColor, thickness, 8, 0);

        cv::cvtColor(image, framebuffer_compat, cv::COLOR_RGB2BGR565);
        for (int y = 0; y < height; y++)
        {
            ofs.seekp(y * width * 2);
            ofs.write(reinterpret_cast<char *>(framebuffer_compat.ptr(y)), width * 2);
        }
    }

}
