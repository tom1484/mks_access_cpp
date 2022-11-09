#include <iostream>
#include <ctime>
#include <unistd.h>
#include <string>
#include <vector>
#include <time.h>
#include <thread>

#include <jsoncpp/json/json.h>
#include <opencv2/core/core.hpp>

#include "connection/connection.hpp"
#include "frame/frame.hpp"
#include "serial/serial.hpp"

const int textScale = 8;
const int textThickness = 4;

const std::string BASE_URL = "140.112.174.222:1484/mks_access";

const std::string SERIAL_DEV = "/dev/ttyACM0";
const int SERIAL_BOUD = 9600;
const char SERIAL_START_SYMBOL = '\t';
const char SERIAL_STOP_SYMBOL = '\n';

std::tm currTime;
std::thread currTimeUpdaterThread;

int inputType;
std::string RFID;
std::string studentID;

bool registering;

void currTimeUpdater();
bool currTimeUpdated();
std::vector<std::string> getCurrTimeString();
std::string getTimestamp();

bool parseInput(std::string);

int main()
{

    Connection::initialize(BASE_URL);
    Frame::initialize(0, 1920, 1080);
    Serial::initialize(SERIAL_DEV, SERIAL_BOUD);

    currTimeUpdaterThread = std::thread(currTimeUpdater);

    Serial::setMessageWrapper(SERIAL_START_SYMBOL, SERIAL_STOP_SYMBOL);
    Serial::startScanner();

    registering = false;
    while (true)
    {
        if (Serial::available())
        {
            std::string input = Serial::getInput();
            std::string timestamp = getTimestamp();
            if (parseInput(input))
            {
                if (!registering)
                {
                    if (inputType == 0)
                    {
                        Json::Value result = Connection::sendUpdateRequest(RFID, timestamp);
                        bool flag = result["flag"].asBool();
                        if (flag)
                        {
                            std::string displayName = result["personalInfo"]["displayName"].asString();
                            std::string welcomeMessage = displayName;
                            std::cout << welcomeMessage << std::endl;

                            Frame::unlock();
                            Frame::lock(3000);
                            Frame::showLine(
                                welcomeMessage, cv::Scalar(0, 0, 0), cv::Scalar(0, 255, 0),
                                textScale, textThickness);
                        }
                        else
                        {
                            std::string requireRegisterMessage = "Please register!";
                            std::cout << requireRegisterMessage << std::endl;

                            Frame::unlock();
                            Frame::lock(3000);
                            Frame::showLine(
                                requireRegisterMessage, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 0),
                                textScale, textThickness);

                            registering = true;
                        }
                    }
                }
                else
                {
                    if (inputType == 1)
                    {
                        Json::Value result = Connection::sendRegisterRequest(RFID, studentID, timestamp);
                        bool flag = result["flag"].asBool();
                        if (flag)
                        {
                            std::string displayName = result["personalInfo"]["displayName"].asString();
                            std::string welcomeMessage = displayName;
                            std::cout << welcomeMessage << std::endl;

                            Frame::unlock();
                            Frame::lock(3000);
                            Frame::showLine(
                                welcomeMessage, cv::Scalar(0, 0, 0), cv::Scalar(0, 255, 0),
                                textScale, textThickness);

                            registering = false;
                        }
                        else
                        {
                            std::string registerFailureMessage = "Failed to register!";
                            std::cout << registerFailureMessage << std::endl;

                            Frame::unlock();
                            Frame::lock(3000);
                            Frame::showLine(
                                registerFailureMessage, cv::Scalar(0, 0, 0), cv::Scalar(255, 0, 0),
                                textScale, textThickness);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

bool parseInput(std::string input)
{
    if (input.length() >= 3 && input[0] == '0')
    {
        inputType = 0;
        RFID = input.substr(2);
        if (RFID.length() == 8)
        {
            return true;
        }
    }
    else if (input.length() >= 3 && input[0] == '1')
    {
        inputType = 1;
        studentID = input.substr(2);
        if (studentID.length() == 9)
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

void currTimeUpdater()
{
    while (true)
    {
        if (currTimeUpdated())
        {
            std::vector<std::string> currTimeString = getCurrTimeString();
            Frame::showLines(currTimeString, cv::Scalar(255, 0, 0), cv::Scalar(255, 255, 0), 200, -1);
        }
    }
}

bool currTimeUpdated()
{

    std::time_t t = std::time(0);
    t += 8 * (60 * 60);
    std::tm now = *std::localtime(&t);

    if (currTime.tm_sec != now.tm_sec)
    {
        currTime = now;
        return true;
    }

    return false;
}

std::string getTimestamp()
{
    std::string year = std::to_string(currTime.tm_year + 1900);
    std::string mon = std::to_string(currTime.tm_mon + 1);
    std::string mday = std::to_string(currTime.tm_mday);
    std::string hour = std::to_string(currTime.tm_hour);
    std::string min = std::to_string(currTime.tm_min);
    std::string sec = std::to_string(currTime.tm_sec);

    mon = std::string(mon.length() == 1 ? "0" : "") + mon;
    mday = std::string(mday.length() == 1 ? "0" : "") + mday;
    hour = std::string(hour.length() == 1 ? "0" : "") + hour;
    min = std::string(min.length() == 1 ? "0" : "") + min;
    sec = std::string(sec.length() == 1 ? "0" : "") + sec;

    return year + "-" + mon + "-" + mday + " " + hour + ":" + min + ":" + sec;
}

std::vector<std::string> getCurrTimeString()
{
    std::string year = std::to_string(currTime.tm_year + 1900);
    std::string mon = std::to_string(currTime.tm_mon + 1);
    std::string mday = std::to_string(currTime.tm_mday);
    std::string hour = std::to_string(currTime.tm_hour);
    std::string min = std::to_string(currTime.tm_min);
    std::string sec = std::to_string(currTime.tm_sec);

    mon = std::string(mon.length() == 1 ? "0" : "") + mon;
    mday = std::string(mday.length() == 1 ? "0" : "") + mday;
    hour = std::string(hour.length() == 1 ? "0" : "") + hour;
    min = std::string(min.length() == 1 ? "0" : "") + min;
    sec = std::string(sec.length() == 1 ? "0" : "") + sec;

    std::vector<std::string> stringVec;
    stringVec.push_back(year + "-" + mon + "-" + mday);
    stringVec.push_back(hour + ":" + min + ":" + sec);

    return stringVec;
}
