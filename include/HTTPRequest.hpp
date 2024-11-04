#include <string>
#include <unordered_map>
#include <sstream>

class HttpRequest {
public:
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    // O metodă statică de parsare a textului cererii brute într-un obiect HttpRequest
    static HttpRequest parse(const std::string& request_text);
};
