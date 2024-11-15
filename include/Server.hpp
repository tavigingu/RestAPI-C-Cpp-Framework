#pragma once
#include "DynamicThreadPool.hpp"
#include "Router.hpp"
#include "Middleware.hpp"
#include "Deserializer.hpp"
#include <memory>

class Server
{
public:
    Server(bool useDynamicPool = false, int minThreads=2, int maxThreads=8, std::chrono::seconds idleTimeout= std::chrono::seconds(2), const int port=8080);
    Server(const int port, const int numThreads=std::thread::hardware_concurrency()/2);
    

    ~Server() { };

    void start();
    void __add_route__(const std::string& path, const std::string& method, Router::HandlerFunction handler);
    void use(Middleware::MiddlewareFunc middleware);



private:
    int m_server_socket;
    int m_port;
    std::unique_ptr<ThreadPool> m_threadPool;
    Router m_router;
    Middleware m_middleware;


    void initializeSocket();
    void configureSocket();
    void bindSocket();
    void listenOnSocket();
    void handleRequest(int client_socket);
};

