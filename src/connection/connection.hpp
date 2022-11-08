#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

#include <jsoncpp/json/json.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

namespace Connection
{

    void initialize(std::string _baseUrl);

    Json::Value sendUpdateRequest(std::string RFID, std::string timestamp);
    Json::Value sendRegisterRequest(std::string RFID, std::string studentID, std::string timestamp);

};

#endif
