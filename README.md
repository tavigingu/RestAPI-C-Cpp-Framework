# REST API Framework in C++

## Description

This framework simplifies the process of building REST API servers in C++. It offers:
- Route handling with flexible HTTP methods.
- Middleware support for logging, CORS, and more.
- Dynamic or fixed thread pool for optimal performance.
- JSON handling for requests and responses.
- Built-in error handling and status code management.

## Features

- **Dynamic Thread Pool Management:** Choose between fixed or dynamic thread pools.
- **Middleware Integration:** Easily add custom middleware for request preprocessing.
- **Simple Routing System:** Define routes with custom logic for GET, POST, and other HTTP methods.
- **Detailed Error Handling:** Simplifies exception and status code management.
- **Extensibility:** Add new middleware and extend functionalities as needed.

## Usage

Here’s an example of how to set up and run the server:

```cpp

// Custom middleware example
Middleware::MiddlewareFunc CustomLogger = [](const HttpRequest& req, HttpResponse& res, std::function<void()> next) {
    std::cout << "[CustomLogger] " << req.getMethod() << " request to " << req.getPath() << std::endl;
    next();
};

int main() {

    Server* my_server = new Server(true);

    // Add middleware
    my_server->use(CustomLogger);
    my_server->use(MiddlewareLibrary::CORS("http://localhost:8080"));

    // Add routes
    my_server->__add_route__("/", "GET", [](const HttpRequest& req, HttpResponse& res) {
        res.setContentType(ContentType::HTML);
        res.SendHtmlPage("index.html");
    });

    my_server->__add_route__("/data", "GET", [](const HttpRequest& req, HttpResponse& res) {
        nlohmann::json response = {
            {"message", "Hello, world!"},
            {"status", "success"}
        };
        res.setContentType(ContentType::JSON);
        res.Send(response);
    });

    my_server->start();
    return 0;
}
