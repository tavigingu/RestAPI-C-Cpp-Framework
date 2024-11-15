#pragma once
#include <string>
#include <unordered_map>
#include <sstream>
#include <nlohmann.hpp>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <functional>
#include <fstream>
#include <sstream>
#include "ServerException.hpp"
#include "ContentType.hpp"

class HttpResponse {
public:
    HttpResponse(const int __client_socket)
        : client_socket_(__client_socket) { }
 
    void Send(nlohmann::json json_obj);
    void SendHtmlPage(const std::string& filename);
    inline bool isResponded() const { return m_respondend; }

    int getStatusCode() const;
    const std::string& getStatusMessage() const;
    const std::unordered_map<std::string, std::string>& getHeaders() const;
    const std::string& getBody() const;

    void setStatusCode(int status_code);
    void setStatusMessage(const std::string& status_message);
    void setHeader(const std::string& header, const std::string& value);
    void setBody(const std::string& body);

    void setContentType(const ContentType& content_type);
    void setContentType(const std::string& content_type);

    std::string to_string() const {
        std::stringstream ss;

        // Statusul HTTP
        ss << "HTTP/1.1 " << status_code_ << " " << status_message_ << "\r\n";

        // Capetele HTTP
        for (const auto& header : headers_) {
            ss << header.first << ": " << header.second << "\r\n";
        }

        // Dacă există un corp, îl adăugăm
        if (!body_.empty()) {
            ss << "\r\n" << body_;
        }

        return ss.str();
}

private:
    std::string load_html_file(const std::string& filename);

    int client_socket_;
    int status_code_ = 200;
    std::string status_message_ = "OK";
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
    bool m_respondend = false;



};
