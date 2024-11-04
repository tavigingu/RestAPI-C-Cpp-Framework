#include "HTTPRequest.hpp"

HttpRequest HttpRequest::parse(const std::string &request_text)
{
    HttpRequest request;
        std::istringstream request_stream(request_text);

        // Parsăm prima linie (metoda, calea și versiunea HTTP)
        request_stream >> request.method >> request.path;
        std::string version;
        request_stream >> version;

        // Parsăm anteturile
        std::string line;
        while (std::getline(request_stream, line) && line != "\r") {
            auto delimiter_pos = line.find(": ");
            if (delimiter_pos != std::string::npos) {
                std::string header_name = line.substr(0, delimiter_pos);
                std::string header_value = line.substr(delimiter_pos + 2);
                request.headers[header_name] = header_value;
            }
        }

        // Parsăm corpul cererii (dacă există)
        if (request.headers.find("Content-Length") != request.headers.end()) {
            int content_length = std::stoi(request.headers["Content-Length"]);
            std::string body(content_length, '\0');
            request_stream.read(&body[0], content_length);
            request.body = body;
        }

        return request;
}