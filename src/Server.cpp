#include "Server.hpp"

#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>


Server::Server(bool useDynamicPool, int minThreads, int maxThreads, std::chrono::seconds idleTimeout, const int port)
    : m_port(port), m_server_socket(-1)
{   
    if (useDynamicPool) {
        printf("Creating dynamic pool...\n");
        m_threadPool = std::make_unique<DynamicThreadPool>(minThreads, maxThreads, idleTimeout);
    } else {
        printf("Creating fixed pool...\n");
        m_threadPool = std::make_unique<ThreadPool>(); 
    }

    initializeSocket(); 
}

void Server::start()
{
    printf("Server is listening on port %d ... \n", m_port);

    while(true)// Initialize the server socket during construction
    {
        int client_socket=accept(m_server_socket,nullptr,nullptr);
        if(client_socket<0) {
            perror("Accept failed");
            continue;
        }

         printf("Connection accepted on socket %d\n", client_socket);

        m_threadPool->enqueue([this, client_socket](){
            handleRequest(client_socket);
        });
        
    }
}

void Server::add_route(const std::string &path, const std::string &method, Router::HandlerFunction handler)
{
    m_router.add_route(path, method, handler);
}

void Server:: initializeSocket()
{
    m_server_socket=socket(AF_INET, SOCK_STREAM, 0);
    if(m_server_socket<0) {
        perror("Socket creation failed!");
        exit(EXIT_FAILURE);
    }

    configureSocket();
    bindSocket();
    listenOnSocket();
}

void Server::configureSocket()
{   
    int options=1;
    //reuse address and port
    if (setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options, sizeof(options)) < 0) {
        perror("Set socket options failed");
        close(m_server_socket);
        exit(EXIT_FAILURE);
    }
}

void Server::bindSocket()
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); //initialize with 0
    server_addr.sin_family=AF_INET; //IPv4
    server_addr.sin_addr.s_addr=INADDR_ANY; //connections from any ip
    server_addr.sin_port=htons(m_port); //convert port number

    if (bind(m_server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed"); // Print error if binding fails
        close(m_server_socket);    // Close the socket
        exit(EXIT_FAILURE);       // Exit the program
    }
}

void Server::listenOnSocket()
{
    if(listen(m_server_socket, 10) < 0) {
        perror("Listen failed");
        close(m_server_socket);
        exit(EXIT_FAILURE);
    }
}

void Server::handleRequest(int client_socket)
{
    char buffer[1024] = {0}; // store incoming request
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0); // recv data from client

    if (bytes_received <= 0) {
        close(client_socket); 
        return;            
    }

    //printf("Received request:\n%s\n", buffer); 
    printf("Handling request on thread %ld: %s\n", std::this_thread::get_id(), buffer);

    std::string request_text(buffer, bytes_received);

    m_router.route_request(request_text, client_socket);

    close(client_socket); // Close the client socket after responding

}

void Server::sendResponse(int client_socket, const char* content) {
    
    std::string http_response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: " + std::to_string(strlen(content)) + "\r\n"
        "Connection: close\r\n"
        "\r\n" + content;

    send(client_socket, http_response.c_str(), http_response.length(), 0);

    close(client_socket);
}