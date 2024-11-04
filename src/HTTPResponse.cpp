#include "HTTPResponse.hpp"

std::string HttpResponse::to_string() const
{
    std::ostringstream response_stream;
        
        // Adăugăm linia de stare
        response_stream << "HTTP/1.1 " << status_code << " " << status_message << "\r\n";
        
        // Adăugăm anteturile
        for (const auto& header : headers) {
            response_stream << header.first << ": " << header.second << "\r\n";
        }
        
        // Adăugăm un antet de Content-Length, necesar pentru a indica mărimea corpului
        response_stream << "Content-Length: " << body.size() << "\r\n";
        
        // Liniile goale între antet și corp
        response_stream << "\r\n";
        
        // Adăugăm corpul răspunsului
        response_stream << body;
        
        return response_stream.str();
}