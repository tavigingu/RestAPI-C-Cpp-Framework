#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"


class Middleware {
public:

    using MiddlewareFunc = std::function<void(const HttpRequest&, HttpResponse&, std::function<void()>)>;

    inline void use(MiddlewareFunc func) { middlewares_.push_back(func); }

    void run(const HttpRequest& req, HttpResponse& res);


private:
    std::vector<MiddlewareFunc> middlewares_;


};
