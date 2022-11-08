#include "serial.hpp"


namespace Serial {

    void initializeSerial(char* serialDevice, int boudRate) {
        serialStreamId = serialOpen(serialDevice, boudRate);
    }

    void setMessageWrapper(char _startSymbol, char _stopSymbol) {
        startSymbol = _startSymbol;
        stopSymbol = _stopSymbol;
    }

    void serialScanner() {
        char byteIn;
        while (true) {
            byteIn = serialGetchar(serialStreamId);
            if (isScannerEnabled) {
                if (byteIn == '\n') {
                    isScannerEnabled = false;
                    isInputAvailable = true;
                } else {
                    serialInput += byteIn;
                }
            } else if (!isInputAvailable) {
                if (byteIn == startSymbol) {
                    isScannerEnabled = true;
                }
            }
        }
    }

    void startScanner() {
        isScannerEnabled = false;
        isInputAvailable = false;
        serialInput = "";

        serialScannerThread = std::thread(serialScanner);
    }

    bool available() {
        return isInputAvailable;
    }

    std::string getInput() {
        if (isInputAvailable) {
            isInputAvailable = false;
            serialInput = "";
            return serialInput;
        }
        else
            return "";
    }

}
