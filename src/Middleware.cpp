#include "Middleware.hpp"

void Middleware::run(const HttpRequest& req, HttpResponse& res) {

    size_t index = 0;

    std::function<void()> next = [&]() {
        if (index < middlewares_.size()) {
            auto currentMiddleware = middlewares_[index];
            index++;

            currentMiddleware(req, res, next);  // Apelează middleware-ul curent cu next
        }
    };

    next();
}