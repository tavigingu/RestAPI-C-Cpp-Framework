#include "Server.hpp"
#include "HTTPResponse.hpp"


int main() {

    Server* my_server = new Server(false);
    

    my_server->add_route("/hello", "GET", [](const HttpRequest& req) {
    HttpResponse response;
    response.status_code = 200;
    response.status_message = "OK";
    response.body = "<html><body><h1>Hello, World!</h1></body></html>";
    response.headers["Content-Type"] = "text/html";
    return response;
});

    my_server->start();

    return 0;
}
