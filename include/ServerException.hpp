#pragma once
#include <exception>
#include <string>
#include <sstream>

// Clasa de bază pentru toate excepțiile serverului
class ServerException : public std::exception {
public:
    explicit ServerException(const std::string& message) : message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

// Excepție specifică pentru erori de socket
class SocketException : public ServerException {
public:
    explicit SocketException(const std::string& message)
        : ServerException("Socket Error: " + message) {}
};

// Excepție specifică pentru erori de rutare
class RouterException : public ServerException {
public:
    explicit RouterException(const std::string& message)
        : ServerException("Router Error: " + message) {}
};

// Excepție specifică pentru erori de fișiere
class FileException : public ServerException {
public:
    explicit FileException(const std::string& message)
        : ServerException("File Error: " + message) {}
};

class RouteNotFound : public ServerException {
public:
    explicit RouteNotFound(const std::string& route)
        : ServerException("Route Not Found: " + route) {}
};


