#include "serial.hpp"

namespace Serial
{

    bool available();
    std::string getInput();

    char startSymbol;
    char stopSymbol;

    int serialStreamId;
    std::thread serialScannerThread;

    bool isScannerEnabled;
    bool isInputAvailable;
    std::string serialInput;

    void serialScanner()
    {
        char byteIn;
        while (true)
        {
            byteIn = serialGetchar(serialStreamId);
            if (isScannerEnabled)
            {
                if (byteIn == '\n')
                {
                    isScannerEnabled = false;
                    isInputAvailable = true;
                }
                else
                {
                    serialInput += byteIn;
                }
            }
            else if (!isInputAvailable)
            {
                if (byteIn == startSymbol)
                {
                    serialInput = "";
                    isScannerEnabled = true;
                }
            }
        }
    }

    void initialize(std::string serialDevice, int boudRate)
    {
        serialStreamId = serialOpen(serialDevice.c_str(), boudRate);
    }

    void setMessageWrapper(char _startSymbol, char _stopSymbol)
    {
        startSymbol = _startSymbol;
        stopSymbol = _stopSymbol;
    }

    void startScanner()
    {
        isScannerEnabled = false;
        isInputAvailable = false;
        serialInput = "";

        serialScannerThread = std::thread(serialScanner);
    }

    bool available()
    {
        return isInputAvailable;
    }

    std::string getInput()
    {
        if (isInputAvailable)
        {
            isInputAvailable = false;
            return serialInput;
        }
        else
            return "";
    }
};
