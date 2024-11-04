#include "DynamicThreadPool.hpp"
#include "Router.hpp"
#include <memory>

class Server
{
public:
    Server(bool useDynamicPool = false, int minThreads=2, int maxThreads=8, std::chrono::seconds idleTimeout= std::chrono::seconds(2), const int port=8080);
    ~Server() { };

    void start();
    void add_route(const std::string& path, const std::string& method, Router::HandlerFunction handler);


private:
    int m_server_socket;
    int m_port;
    std::unique_ptr<ThreadPool> m_threadPool;
    Router m_router;


    void initializeSocket();
    void configureSocket();
    void bindSocket();
    void listenOnSocket();
    void handleRequest(int client_socket);
    void sendResponse(int client_socket, const char* content);
};

