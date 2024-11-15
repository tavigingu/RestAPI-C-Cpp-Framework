#include "HTTPRequest.hpp"

nlohmann::json HttpRequest::toJson() const {

    if (headers_.count("Content-Type")) {
        std::string contentType = headers_.at("Content-Type");

        contentType.erase(contentType.begin(), std::find_if(contentType.begin(), contentType.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        contentType.erase(std::find_if(contentType.rbegin(), contentType.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), contentType.end());

        if (contentType == "application/x-www-form-urlencoded") {
            return parseBodyAsFormUrlEncoded();
        } else if (contentType == "application/json") {
            return nlohmann::json::parse(body_);
        }
    }
    return nlohmann::json{};
}


nlohmann::json HttpRequest::parseBodyAsFormUrlEncoded() const {
    
    nlohmann::json json_result;
    std::istringstream stream(body_);
    std::string pair;
    
    while (std::getline(stream, pair, '&')) {
        auto pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            
            // Decodează caracterele URL-encoded (ex: %40 -> @)
            key = urlDecode(key);
            value = urlDecode(value);
            
            json_result[key] = value;
        }
    }

    return json_result;
}

// Funcție pentru decodificarea caracterelor URL-encoded
std::string HttpRequest::urlDecode(const std::string& str) const {
    std::string decoded;
    char hex[3];
    hex[2] = '\0';

    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '%') {
            if (i + 2 < str.length()) {
                hex[0] = str[i + 1];
                hex[1] = str[i + 2];
                decoded += static_cast<char>(strtol(hex, nullptr, 16));
                i += 2;
            }
        } else if (str[i] == '+') {
            decoded += ' ';
        } else {
            decoded += str[i];
        }
    }

    return decoded;
}

std::string HttpRequest::getMethod() const {
    return method_;
}

std::string HttpRequest::getPath() const {
    return path_;
}

std::unordered_map<std::string, std::string> HttpRequest::getHeaders() const {
    return headers_;
}

std::string HttpRequest::getHeader(const std::string& header) const
{
   auto it = headers_.find(header); // Caută antetul în map
        if (it != headers_.end()) {
            return it->second;           // Dacă antetul este găsit, returnează valoarea sa
        }
        return ""; // Returnează un string gol dacă antetul nu este găsit
}

std::string HttpRequest::getBody() const {
    return body_;
}

// Settere pentru fiecare membru privat
void HttpRequest::setMethod(const std::string& method) {
    method_ = method;
}

void HttpRequest::setPath(const std::string& path) {
    path_ = path;
}

void HttpRequest::setHeader(const std::string& header, const std::string& value) {
    headers_[header] = value;  // Adaugă sau modifică perechea cheie-valoare
}

void HttpRequest::setBody(const std::string& body) {
    body_ = body;
}

void HttpRequest::setParam(const std::string& key, const std::string& value) {
        url_params_[key] = value;
}

std::string HttpRequest::getParam(const std::string& key) const {
    auto it = url_params_.find(key);
    if (it != url_params_.end()) {
        return it->second;
    }
    
    return "";
    }