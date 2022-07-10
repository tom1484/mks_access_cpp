#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

#include <jsoncpp/json/json.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>


class Connection {

public:

    void initialize(const std::string url);

    Json::Value sendUpdateRequest(const std::string RFID, const std::string timestamp);
    Json::Value sendRegisterRequest(const std::string RFID, const std::string studentID, const std::string timestamp);

private:

    curlpp::Easy request;

    std::string baseUrl;
    std::stringstream resultStream;

    Json::Value parseResult();

};



