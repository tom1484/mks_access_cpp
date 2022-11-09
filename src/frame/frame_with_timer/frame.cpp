#include "frame.hpp"

namespace Frame
{

    std::ofstream ofs;
    int width, height;
    cv::Ptr<cv::freetype::FreeType2> ft2;

    bool toLock;
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

        ft2 = cv::freetype::createFreeType2();
        ft2->loadFontData("./FiraCode.ttf", 0);

        toLock = false;
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
                    std::cout << "Manually unlock" << std::endl;
                }
            }
        }
    }

    void lock(int _lockPeriod)
    {
        if (!frameLocked && !toLock)
        {
            lockPeriod = _lockPeriod;
            toLock = true;
        }
    }

    void unlock()
    {
        if (frameLocked)
        {
            frameLocked = false;
        }
    }

    void printImage(cv::Mat &framebuffer_compat)
    {
        if (frameLocked)
        {
            return;
        }

        if (toLock)
        {
            toLock = false;
            frameLocked = true;
        }
        for (int y = 0; y < height; y++)
        {
            ofs.seekp(y * width * 2);
            ofs.write(reinterpret_cast<char *>(framebuffer_compat.ptr(y)), width * 2);
        }
    }

    void showText(std::string &text, cv::Scalar fontColor, cv::Scalar bgColor, double fontHeight, int thickness)
    {
        if (frameLocked)
        {
            return;
        }

        cv::Mat image = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
        cv::Mat framebuffer_compat;

        // set background
        image.setTo(bgColor);

        std::vector<std::string> lines(0);
        for (int i = 0, p = 0; i <= text.length(); i++)
        {
            if (i == text.length() || text[i] == '\n')
            {
                lines.push_back(text.substr(p, i - p));
                p = i + 1;
            }
        }

        for (int i = 0; i < lines.size(); i++)
        {
            std::string text = lines[i];
            int baseline = 0;

            // get text size
            cv::Size text_size = ft2->getTextSize(text, fontHeight, thickness, &baseline);

            cv::Point origin;
            origin.x = image.cols / 2 - text_size.width / 2;
            origin.y = image.rows / 2 + text_size.height / 2 - int((text_size.height + 40) * (i - (lines.size() - 1) / 2.0));

            ft2->putText(image, text, origin, fontHeight, fontColor, thickness, 8, true);
        }

        if (frameLocked)
        {
            return;
        }
        cv::cvtColor(image, framebuffer_compat, cv::COLOR_RGB2BGR565);
        printImage(framebuffer_compat);
    }

}
