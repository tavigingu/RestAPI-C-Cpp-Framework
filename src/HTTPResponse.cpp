#include "HTTPResponse.hpp"
#include "Serializer.hpp"


void HttpResponse::Send(nlohmann::json json_obj)
{
    body_ = json_obj.dump();
    std::string response_text = Serializer::serialize(*this);

    send(client_socket_, response_text.c_str(), response_text.size(), 0);
}

void HttpResponse::SendHtmlPage(const std::string &filename)
{
    body_ = load_html_file(filename);
    std::string response_text = Serializer::serialize(*this);

    send(client_socket_, response_text.c_str(), response_text.size(), 0);
}

std::string HttpResponse::load_html_file(const std::string &filename)
{
     try {
        std::ifstream file("html/" + filename);
        if (!file.is_open()) {
            throw FileException("Could not open file: " + filename);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();

    } catch (const FileException& e) {
        std::cerr << "File loading error: " << e.what() << std::endl;
        
        return "<html><body><h1>404 Not Found</h1><p>Page not found.</p></body></html>";
    }
}

int HttpResponse::getStatusCode() const {
    return status_code_;
}

const std::string& HttpResponse::getStatusMessage() const {
    return status_message_;
}

const std::unordered_map<std::string, std::string>& HttpResponse::getHeaders() const {
    return headers_;
}

const std::string& HttpResponse::getBody() const {
    return body_;
}

// Setters
void HttpResponse::setStatusCode(int code) {
    status_code_ = code;
}

void HttpResponse::setStatusMessage(const std::string& message) {
    status_message_ = message;
}

void HttpResponse::setHeader(const std::string& header, const std::string& value) {
    headers_[header] = value;
}

void HttpResponse::setBody(const std::string& body_content) {
    body_ = body_content;
}

void HttpResponse::setContentType(const std::string &content_type)
{
    setHeader("Content-Type", content_type);
}

void HttpResponse::setContentType(const ContentType& content_type)
{
    setHeader("Content-Type", to_string(content_type));
}
