#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>

#include <iostream>
#include <string>
#include <thread>

namespace Serial {

    void initializeSerial(char* serialDevice, int boudRate);
    void setMessageWrapper(char _startSymbol, char _stopSymbol);
    void serialScanner();
    void startScanner();
    
    bool available();
    std::string getInput();

    char startSymbol;
    char stopSymbol;

    int serialStreamId;
    std::thread serialScannerThread;

    bool isScannerEnabled;
    bool isInputAvailable;
    std::string serialInput;
    
}
