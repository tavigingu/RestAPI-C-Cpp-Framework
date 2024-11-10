#pragma once
#include <string>

enum class ContentType {
    HTML,
    JSON,
    TEXT,
    XML,
    PNG,
    JPEG
};


inline std::string to_string(ContentType content_type) {
    switch (content_type) {
        case ContentType::HTML:       return "text/html";
        case ContentType::JSON: return "application/json";
        case ContentType::TEXT:       return "text/plain";
        case ContentType::XML:  return "application/xml";
        case ContentType::PNG:        return "image/png";
        case ContentType::JPEG:       return "image/jpeg";
        default:                            return "application/octet-stream";
    }
}
