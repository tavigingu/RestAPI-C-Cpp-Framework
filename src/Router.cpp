#include "Router.hpp"
#include "ServerException.hpp"
#include "Serializer.hpp"
#include "Deserializer.hpp"
#include <iostream>

void Router::route_request(const std::string &request_text, int client_socket)
{
        HttpRequest request = Deserializer::deserialize_request(request_text);
        
        std::string key = request.getMethod() + " " + request.getPath();
        
        HttpResponse response(client_socket);


        if (routes_.find(key) != routes_.end()) {
            routes_[key](request, response);
        } else {
            return;
        }
  
}
