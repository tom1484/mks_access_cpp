#include <iostream>
#include <jsoncpp/json/json.h>

#include "connection/connection.hpp"

Connection conn;
const std::string BASE_URL = "140.112.174.222:1484/mks_access";

int main() {

    conn.initialize(BASE_URL);

    std::string RFID = "A5280939";
    std::string studentID = "B10901002";
    std::string timestamp = "2022-07-10 10:41:00";

    Json::Value result;
    result = conn.sendUpdateRequest(RFID, timestamp);
    std::cout << result << std::endl;
    result = conn.sendRegisterRequest(RFID, studentID, timestamp);
    std::cout << result << std::endl;

    return 0;
}
