#include "Router.hpp"

void Router::route_request(const std::string &request_text, int client_socket)
{
    HttpRequest request = HttpRequest::parse(request_text);
        std::string key = request.method + " " + request.path;
        
        HttpResponse response;
        if (routes_.find(key) != routes_.end()) {
            response = routes_[key](request);
        } else {
            // Răspuns 404 dacă ruta nu este găsită
            response.status_code = 404;
            response.status_message = "Not Found";
            response.body = "404 - Not Found";
        }

        // Trimitere răspuns HTTP către client
        std::string response_text = response.to_string();
        send(client_socket, response_text.c_str(), response_text.size(), 0);
}
