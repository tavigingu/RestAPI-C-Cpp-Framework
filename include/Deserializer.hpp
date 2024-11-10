#include "HTTPRequest.hpp"
#include <sstream>
#include <string>
#include <unordered_map>
#include <iostream>

class Deserializer {
public:
    static HttpRequest deserialize_request(const std::string& request_text) {
        
        HttpRequest request;
        
        std::istringstream stream(request_text);
        std::string line;

        if (std::getline(stream, line)) {
            std::istringstream request_line_stream(line);
            std::string method;
            std::string path;
            std::string http_version;
            
            request_line_stream >> method; 
            request_line_stream >> path;  
            request_line_stream >> http_version;   
            
            request.setMethod(method);
            request.setPath(path);

        }

        while (std::getline(stream, line) && line != "\r" && line != "") {
            auto colon_pos = line.find(":");
            if (colon_pos != std::string::npos) {
                std::string header_name = line.substr(0, colon_pos);
                std::string header_value = line.substr(colon_pos + 1);
                
                header_name.erase(header_name.find_last_not_of(" \t\r\n") + 1);
                header_value.erase(0, header_value.find_first_not_of(" \t\r\n"));              

                request.setHeader(header_name, header_value);

                printf("  %s: %s\n", header_name.c_str(), header_value.c_str());
            }
        }

        std::ostringstream body_stream;
        while (std::getline(stream, line)) {
            body_stream << line << "\n";
        }
        std::string body = body_stream.str();
        
        if (!body.empty() && body.back() == '\n') {
            body.pop_back();
        }

        request.setBody(body);

        return request;
    }
};
