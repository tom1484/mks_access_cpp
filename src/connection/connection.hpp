#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

#include <jsoncpp/json/json.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>


namespace Connection {

    void initialize(const std::string _baseUrl);

    Json::Value sendUpdateRequest(const std::string RFID, const std::string timestamp);
    Json::Value sendRegisterRequest(const std::string RFID, const std::string studentID, const std::string timestamp);

};



