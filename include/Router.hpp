#pragma once
#include <unordered_map>
#include <string>
#include <unistd.h>
#include <functional>
#include <sstream>
#include <sys/socket.h> 
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class Router {
public:
    using HandlerFunction = std::function<HttpResponse(const HttpRequest&)>;

    void add_route(const std::string& route, const std::string& method, HandlerFunction handler) {
        routes_[method + " " + route] = handler;
    }

    void route_request(const std::string& request_text, int client_socket);

private:
    std::unordered_map<std::string, HandlerFunction> routes_;
};
