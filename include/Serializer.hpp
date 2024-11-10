#pragma once
#include <string>
#include "HTTPResponse.hpp"
#include <nlohmann.hpp>

class Serializer {
public:
    static std::string serialize(HttpResponse& response) {
        
        std::stringstream response_stream;

        response_stream << "HTTP/1.1 " << response.getStatusCode() << " " << response.getStatusMessage()<< "\r\n";

        for (const auto& header : response.getHeaders()) {
            response_stream << header.first << ": " << header.second << "\r\n";
        }

        response_stream << "\r\n";

        response_stream << response.getBody();

        return response_stream.str();  // Returnăm rezultatul ca un string
    }

};
