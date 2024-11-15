#include <iostream>
#include <string>
#include "Server.hpp"

class MiddlewareLibrary {
public:
    // Logger Middleware
    static Middleware::MiddlewareFunc Logger() {
        return [](const HttpRequest& req, HttpResponse& res, std::function<void()> next) {
            std::cout << "[LOG] " << req.getMethod() << " " << req.getPath() << std::endl;
            next();  // Continuăm execuția
        };
    }


    // CORS Middleware
    static Middleware::MiddlewareFunc CORS(const std::string& allowedOrigin = "*") {
        return [allowedOrigin](const HttpRequest& req, HttpResponse& res, std::function<void()> next) {
            res.setHeader("Access-Control-Allow-Origin", allowedOrigin);
            res.setHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
            res.setHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
            next();
        };
    }

    //Authenticator Middleware
    static Middleware::MiddlewareFunc Authenticator(std::function<bool(const std::string&)> validateToken) {
        return [validateToken](const HttpRequest& req, HttpResponse& res, std::function<void()> next) {
            std::string token = req.getHeader("Authorization");
            if (!token.empty() && validateToken(token)) {
                next();  // Autentificare reușită, continuăm
            } else {
                res.setStatusCode(401);
                res.setStatusMessage("Unauthorized");
                res.Send({{"error", "Invalid or missing token"}});
            }
        };
    }
};
