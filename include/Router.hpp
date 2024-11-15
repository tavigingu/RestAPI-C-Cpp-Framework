#pragma once
#include <unordered_map>
#include <string>
#include <unistd.h>
#include <functional>
#include <sstream>
#include <sys/socket.h> 
#include <regex>
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class Router {
public:
    using HandlerFunction = std::function<void(const HttpRequest&, HttpResponse&)>;

    void add_route(const std::string& route, const std::string& method, HandlerFunction handler) {
        routes_[method + " " + route] = handler;
    }

    void route_request(HttpRequest& request, HttpResponse& response);

private:
    void add_params_to_request(const std::string &route_pattern, const std::string &url, HttpRequest& request);
    std::string convertToRegex(const std::string& route);

    std::unordered_map<std::string, HandlerFunction> routes_;
};
