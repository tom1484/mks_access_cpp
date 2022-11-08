#include "serial.hpp"
#include <iostream>

int main() {
    Serial::initializeSerial("/dev/ttyACM0", 9600);
    Serial::setMessageWrapper('H', '\n');
    Serial::startScanner();

    while (true) {
        if (Serial::available()) {
            std::cout << Serial::getInput() << std::endl;
        }
    }

    return 0;
}