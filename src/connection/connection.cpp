#include "connection.hpp"

namespace Connection
{

    curlpp::Easy request;

    std::string baseUrl;
    std::stringstream resultStream;

    Json::Value parseResult();

    void initialize(std::string _baseUrl)
    {

        curlpp::initialize();

        // set base url string
        baseUrl = _baseUrl.back() != '/' ? _baseUrl + '/' : _baseUrl;

        // set HTTP header
        std::list<std::string> header = {
            "Content-Type: application/json",
            "accept: application/json"};
        request.setOpt(new curlpp::options::HttpHeader(header));

        // set request result data stream
        curlpp::options::WriteStream ws(&resultStream);
        request.setOpt(ws);
    }

    Json::Value sendUpdateRequest(std::string RFID, std::string timestamp)
    {

        // set url route
        std::string registerUrl = baseUrl + "update";
        request.setOpt(curlpp::Options::Url(registerUrl));

        // set JSON data
        char query[100];
        const char format[100] =
            "{"
            "\"RFID\": \"%s\", "
            "\"timestamp\": \"%s\""
            "}";
        std::sprintf(query, format, RFID.c_str(), timestamp.c_str());
        request.setOpt(new curlpp::options::PostFields(std::string(query)));

        // send request and decode result
        request.perform();

        return parseResult();
    }

    Json::Value sendRegisterRequest(std::string RFID, std::string studentID, std::string timestamp)
    {

        // set url route
        std::string registerUrl = baseUrl + "register";
        request.setOpt(curlpp::Options::Url(registerUrl));

        // set JSON data
        char query[100];
        const char format[100] =
            "{"
            "\"RFID\": \"%s\", "
            "\"studentID\": \"%s\", "
            "\"timestamp\": \"%s\""
            "}";
        std::sprintf(query, format, RFID.c_str(), studentID.c_str(), timestamp.c_str());
        request.setOpt(new curlpp::options::PostFields(std::string(query)));

        // send request and decode result
        request.perform();

        return parseResult();
    }

    Json::Value parseResult()
    {

        std::string jsonRaw;

        Json::Value resultJson;
        Json::CharReaderBuilder builder;

        builder["collectComments"] = true;
        JSONCPP_STRING errs;

        if (!parseFromStream(builder, resultStream, &resultJson, &errs))
        {
            std::cout << errs << std::endl;
        }

        return resultJson;
    }

};
