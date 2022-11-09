#include <iostream>
#include <ctime>
#include <unistd.h>
#include <string>
#include <vector>
#include <time.h>
#include <thread>
#include <chrono>

#include <jsoncpp/json/json.h>
#include <opencv2/core/core.hpp>

#include "connection/connection.hpp"
#include "frame/frame.hpp"
#include "serial/serial.hpp"

const int textHeight = 200;
const int textThickness = -1;

const std::string BASE_URL = "140.112.174.222:1484/mks_access";

const std::string SERIAL_DEV = "/dev/ttyACM0";
const int SERIAL_BOUD = 9600;
const char SERIAL_START_SYMBOL = '\t';
const char SERIAL_STOP_SYMBOL = '\n';

std::tm currTime;
std::thread currTimeUpdaterThread;

void currTimeUpdater();
bool currTimeUpdated();
std::string getCurrTimeString();
std::string getTimestamp();

int inputType;
std::string RFID = "";
std::string studentID = "";
bool parseInput(std::string);

int timerID;
std::thread timerThread;
void timer(int ID);
void startTimer(std::function<void()> _targetFunc, int _millis);

bool registering;
bool allowDuplicateRFID;

int main()
{

    Connection::initialize(BASE_URL);
    Frame::initialize(0, 1920, 1080);
    Serial::initialize(SERIAL_DEV, SERIAL_BOUD);

    currTimeUpdaterThread = std::thread(currTimeUpdater);

    Serial::setMessageWrapper(SERIAL_START_SYMBOL, SERIAL_STOP_SYMBOL);
    Serial::startScanner();

    registering = false;
    allowDuplicateRFID = true;
    while (true)
    {
        if (Serial::available())
        {
            std::string input = Serial::getInput();
            std::string timestamp = getTimestamp();
            if (parseInput(input))
            {
                // if (!registering)
                // {
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
                        Frame::lock();
                        Frame::showText(
                            welcomeMessage, cv::Scalar(0, 0, 0), cv::Scalar(0, 255, 0),
                            textHeight, textThickness);

                        allowDuplicateRFID = false;
                        startTimer(
                            []()
                            {
                                Frame::unlock();
                                allowDuplicateRFID = true;
                            },
                            3000);
                    }
                    else
                    {
                        std::string requireRegisterMessage = "Please register!";
                        std::cout << requireRegisterMessage << std::endl;

                        Frame::unlock();
                        Frame::lock();
                        Frame::showText(
                            requireRegisterMessage, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 0),
                            textHeight, textThickness);

                        allowDuplicateRFID = false;
                        registering = true;
                        startTimer(
                            []()
                            {
                                Frame::unlock();
                                allowDuplicateRFID = true;
                                registering = false;
                            },
                            5000);
                    }
                }
                // }
                // else
                // {
                if (inputType == 1)
                {
                    if (registering)
                    {
                        Json::Value result = Connection::sendRegisterRequest(RFID, studentID, timestamp);
                        bool flag = result["flag"].asBool();
                        if (flag)
                        {
                            std::string displayName = result["personalInfo"]["displayName"].asString();
                            std::string welcomeMessage = displayName;
                            std::cout << welcomeMessage << std::endl;

                            Frame::unlock();
                            Frame::lock();
                            Frame::showText(
                                welcomeMessage, cv::Scalar(0, 0, 0), cv::Scalar(0, 255, 0),
                                textHeight, textThickness);

                            allowDuplicateRFID = false;
                            registering = false;
                            startTimer(
                                []()
                                {
                                    Frame::unlock();
                                    allowDuplicateRFID = true;
                                },
                                3000);
                        }
                        else
                        {
                            std::string registerFailureMessage = "Failed to register!";
                            std::cout << registerFailureMessage << std::endl;

                            Frame::unlock();
                            Frame::lock();
                            Frame::showText(
                                registerFailureMessage, cv::Scalar(0, 0, 0), cv::Scalar(255, 0, 0),
                                textHeight, textThickness);

                            allowDuplicateRFID = true;
                            registering = false;
                            startTimer(
                                []()
                                {
                                    Frame::unlock();
                                },
                                3000);
                        }
                    }
                }
                // }
            }
        }
    }

    return 0;
}

void startTimer(std::function<void()> _targetFunc, int _millis)
{
    timerID++;
    timerThread = std::thread(
        [&](int ID, int millis, std::function<void()> targetFunc)
        {
            auto startClock = std::chrono::high_resolution_clock::now();
            while (true)
            {
                auto elapsedClock = std::chrono::high_resolution_clock::now() - startClock;
                auto elapsedMillis = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedClock).count();
                if (elapsedMillis >= millis)
                {
                    if (ID == timerID)
                    {
                        targetFunc();
                    }
                    return;
                }
            }
        },
        timerID, _millis, _targetFunc);
    timerThread.detach();
}

bool parseInput(std::string input)
{
    if (input.length() >= 3 && input[0] == '0')
    {
        int _inputType = 0;
        std::string _RFID = input.substr(2);
        if (_RFID.length() == 8 && (allowDuplicateRFID || _RFID != RFID))
        {
            inputType = _inputType;
            RFID = _RFID;
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
    return false;
}

void currTimeUpdater()
{
    while (true)
    {
        if (currTimeUpdated())
        {
            std::string currTimeString = getCurrTimeString();
            Frame::showText(
                currTimeString, cv::Scalar(255, 255, 255), cv::Scalar(0, 0, 0),
                textHeight, textThickness);
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

std::string getCurrTimeString()
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

    std::string result = "";
    result += year + "-" + mon + "-" + mday + "\n";
    result += hour + ":" + min + ":" + sec;

    return result;
}
