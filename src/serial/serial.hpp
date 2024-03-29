#ifndef SERIAL_HPP_
#define SERIAL_HPP_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>

#include <iostream>
#include <string>
#include <thread>

namespace Serial
{

    void initialize(std::string serialDevice, int boudRate);
    void setMessageWrapper(char _startSymbol, char _stopSymbol);
    void serialScanner();
    void startScanner();

    bool available();
    std::string getInput();

}

#endif