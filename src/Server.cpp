#include "Server.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include <fstream>
#include <sstream>
#include "ServerException.hpp"

Server::Server(bool useDynamicPool, int minThreads, int maxThreads, std::chrono::seconds idleTimeout, const int port)
    : m_port(port), m_server_socket(-1)
{
    try {
        if (useDynamicPool) {
            printf("Creating dynamic pool...\n");
            m_threadPool = std::make_unique<DynamicThreadPool>(minThreads, maxThreads, idleTimeout);
        } else {
            printf("Creating fixed pool...\n");
            m_threadPool = std::make_unique<ThreadPool>(); 
        }
        
        initializeSocket(); 
    } catch (const SocketException& e) {
        std::cerr << "Failed to initialize server socket: " << e.what() << std::endl;
        exit(EXIT_FAILURE);  // Terminate if socket initialization fails
    }
}

Server::Server(const int port, const int numThreads)
    :m_port(port), m_server_socket(-1)
{   
    try { 
        m_threadPool = std::make_unique<ThreadPool>(numThreads); 
        initializeSocket(); 
     } catch (const SocketException& e) {
        std::cerr << "Failed to initialize server socket: " << e.what() << std::endl;
        exit(EXIT_FAILURE);  // Terminate if socket initialization fails
    }
}


void Server::start()
{
    printf("Server is listening on port %d ... \n", m_port);

    while (true) {
        try {
            int client_socket = accept(m_server_socket, nullptr, nullptr);
            if (client_socket < 0) {
                throw SocketException("Accept failed");
            }

            printf("Connection accepted on socket %d\n", client_socket);

            m_threadPool->enqueue([this, client_socket]() {
                try {
                    handleRequest(client_socket);
                } catch (const RouterException& e) {
                    std::cerr << "Error handling request: " << e.what() << std::endl;
                }
            });


        } catch (const SocketException& e) {
            std::cerr << "Socket exception occurred: " << e.what() << std::endl;
        }
    }
}

void Server::__add_route__(const std::string &path, const std::string &method, Router::HandlerFunction handler)
{
    m_router.add_route(path, method, handler);
}

void Server::initializeSocket()
{
    m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_socket < 0) {
        throw SocketException("Socket creation failed!");
    }

    configureSocket();
    bindSocket();
    listenOnSocket();
}

void Server::configureSocket()
{
    int options = 1;
    if (setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options, sizeof(options)) < 0) {
        close(m_server_socket);
        throw SocketException("Set socket options failed");
    }
}

void Server::bindSocket()
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(m_port);

    if (bind(m_server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(m_server_socket);
        throw SocketException("Binding failed");
    }
}

void Server::listenOnSocket()
{
    if (listen(m_server_socket, 10) < 0) {
        close(m_server_socket);
        throw SocketException("Listen failed");
    }
}

void Server::handleRequest(int client_socket)
{
    try {
        char buffer[1024] = {0};
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            return;
        }

        printf("Handling request on thread %ld: %s\n", std::this_thread::get_id(), buffer);

        std::string request_text(buffer, bytes_received);

        HttpRequest request = Deserializer::deserialize_request(request_text);
        HttpResponse response(client_socket);

        m_middleware.run(request, response);

        if (response.isResponded()) {
            return;
        }


        m_router.route_request(request, response);


        close(client_socket);  // Close after responding

    } catch (const RouterException& e) {
        std::cerr << "Routing error: " << e.what() << std::endl;
        close(client_socket);
    }
}

void Server::use(Middleware::MiddlewareFunc middleware) {
        m_middleware.use(middleware);  
    }


