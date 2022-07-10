#include <iostream>
#include <ctime>
#include <unistd.h>
#include <string>
#include <jsoncpp/json/json.h>

#include "connection/connection.hpp"
#include "frame/frame.hpp"

const std::string BASE_URL = "140.112.174.222:1484/mks_access";

std::tm currTime;

Connection conn;
Frame frame;

bool updateCurrTime();
std::string getCurrTimeString();

int main() {

    conn.initialize(BASE_URL);
    frame.initialize(0, 1920, 1080);

    // std::string RFID = "A5280939";
    // std::string studentID = "B10901002";
    // std::string timestamp = "2022-07-10 10:41:00";

    // Json::Value result;
    // result = conn.sendUpdateRequest(RFID, timestamp);
    // std::cout << result << std::endl;
    // result = conn.sendRegisterRequest(RFID, studentID, timestamp);
    // std::cout << result << std::endl;
    
    while (true) {
        if (updateCurrTime()) {
            std::cout << getCurrTimeString() << std::endl;
            frame.showCurrentTime(getCurrTimeString());
        }
    }

    return 0;
}

bool updateCurrTime() {
    std::time_t t = std::time(0);
    std::tm now = *std::localtime(&t);

    if (currTime.tm_sec != now.tm_sec) {
        currTime = now;
        return true;
    }
    
    return false;
}

std::string getCurrTimeString() {

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

    std::string currTimeString = year + "-" + mon + "-" + mday + " "
                               + hour + ":" + min + ":" + sec;
    
    return currTimeString;
}