#include "frame.hpp"

namespace Frame
{

    std::ofstream ofs;
    int width, height;

    bool frameLocked;
    int lockPeriod;
    bool timerRunning;
    std::thread timerThread;

    void initialize(int _fb, int _width, int _height)
    {

        // set frame dimension
        width = _width;
        height = _height;

        // set framebuffer stream
        std::string fbDevice = "/dev/fb";
        fbDevice += std::to_string(_fb);
        ofs.open(fbDevice.c_str());

        frameLocked = false;
        timerRunning = false;
        timerThread = std::thread(timer);
    }

    void timer()
    {
        std::chrono::_V2::system_clock::time_point startClock;
        int milliseconds;

        while (true)
        {
            if (frameLocked)
            {
                if (!timerRunning)
                {
                    timerRunning = true;
                    startClock = std::chrono::high_resolution_clock::now();
                }
                else
                {
                    auto elapsedClock = std::chrono::high_resolution_clock::now() - startClock;
                    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedClock).count();

                    if (lockPeriod <= milliseconds)
                    {
                        frameLocked = false;
                        timerRunning = false;
                        std::cout << "Unlock after " << milliseconds << std::endl;
                    }
                }
            }
            else
            {
                if (timerRunning)
                {
                    timerRunning = false;
                    // std::cout << "Forced unlock" << std::endl;
                }
            }
        }
    }

    void lock(int _lockPeriod)
    {
        if (!frameLocked)
        {
            lockPeriod = _lockPeriod;
            frameLocked = true;
        }
    }

    void unlock()
    {
        if (frameLocked)
        {
            frameLocked = false;
        }
    }

    void showLines(std::vector<std::string> textVec, cv::Scalar fontColor, cv::Scalar bgColor, double fontHeight, int thickness)
    {
        // std::cout << frameLocked << std::endl;
        if (frameLocked)
        {
            return;
        }
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

        if (frameLocked)
        {
            return;
        }
        cv::cvtColor(image, framebuffer_compat, cv::COLOR_RGB2BGR565);
        for (int y = 0; y < height; y++)
        {
            ofs.seekp(y * width * 2);
            ofs.write(reinterpret_cast<char *>(framebuffer_compat.ptr(y)), width * 2);
        }
    }

    void showLine(std::string text, cv::Scalar fontColor, cv::Scalar bgColor, double scale, int thickness)
    {

        if (frameLocked)
        {
            return;
        }
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

        if (frameLocked)
        {
            return;
        }
        cv::cvtColor(image, framebuffer_compat, cv::COLOR_RGB2BGR565);
        for (int y = 0; y < height; y++)
        {
            ofs.seekp(y * width * 2);
            ofs.write(reinterpret_cast<char *>(framebuffer_compat.ptr(y)), width * 2);
        }
    }

}
