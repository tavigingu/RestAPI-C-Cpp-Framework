#pragma once
#include <string>
#include <unordered_map>
#include <sstream>

class HttpResponse {
public:
    int status_code = 200;
    std::string status_message = "OK";
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    // Metodă pentru a genera răspunsul HTTP ca text
    std::string to_string() const;
};
