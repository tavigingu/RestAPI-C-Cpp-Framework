#pragma once
#include <string>
#include <unordered_map>
#include <sstream>
#include <nlohmann.hpp> 

class HttpRequest {
public:
    HttpRequest() = default;

    nlohmann::json toJson() const;

    std::string getMethod() const;
    std::string getPath() const;
    std::unordered_map<std::string, std::string> getHeaders() const;
    std::string getBody() const;

    void setMethod(const std::string& method);
    void setPath(const std::string& path);
    void setHeader(const std::string& header, const std::string& value);
    void setBody(const std::string& body);


private:
    nlohmann::json parseBodyAsFormUrlEncoded() const;
    std::string urlDecode(const std::string& str) const;


    std::string method_;
    std::string path_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;

};
