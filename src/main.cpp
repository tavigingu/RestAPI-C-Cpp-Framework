#include "Server.hpp"
#include "HTTPResponse.hpp"
#include "ServerException.hpp"
#include "Serializer.hpp"
#include "MiddlewareLibrary.hpp"
#include "MyDB.hpp"


//TO DO:
// 1. Adaugare functionalitati pentru developer de a gestiona numarul de thread-uri
// 2. Creare metode de DELETE si PUT
// 3. Extrindere middleware + extindere middlewareLibrary
// 4. Gestionarea exceptiilor in detaliu
// 5. Gestionarea status codeurilor astfel incat dezvoltatorul sa isi faca cat mai putine griji despre gestionarea acestora
// 6. Logger (prin intermediul middleware-ului)
// 7. Funtionalitati de conectare la diferite baze de date

Middleware::MiddlewareFunc CustomLogger = [](const HttpRequest& req, HttpResponse& res, std::function<void()> next) {
    std::cout << "[CustomLogger] " << req.getMethod() << " request to " << req.getPath() << std::endl;
    next();
};

int main() {
    
    try {
        
        Server* my_server = new Server(true);


        my_server->use(CustomLogger);
        my_server->use(MiddlewareLibrary::CORS("http://localhost:8080"));
        
              
        my_server->__add_route__("/", "GET", [](const HttpRequest& req, HttpResponse& res) {
            
            res.setContentType(ContentType::HTML);
            res.SendHtmlPage("login.html");
            
        });

    
        my_server->__add_route__("/signup", "POST", [](const HttpRequest& req, HttpResponse& res) {
            
            std::string email = req.getParam("email");
            std::string password = req.getParam("password");

            MyDB::getInstance().addUser(email, password);

            res.setStatusCode(201);
            res.setStatusMessage("Created");
            res.setContentType(ContentType::JSON);
            nlohmann::json response = {
                {"message", "Registration successful!"},
                {"redirect", "/main"}
            };
            res.Send(response);

        });


        my_server->__add_route__("/get_users", "GET", [](const HttpRequest& req, HttpResponse& res) {
            
            nlohmann::json json_users = {
                {{"id", 1}, {"name", "John"}, {"email", "john@example.com"}, {"password", "password123"}},
                {{"id", 2}, {"name", "Alice"}, {"email", "alice@example.com"}, {"password", "alicepass456"}},
                {{"id", 3}, {"name", "Bob"}, {"email", "bob@example.com"}, {"password", "bobspass789"}}
            };
            
            res.setContentType(ContentType::JSON);
            res.Send(json_users);
        });


        my_server->__add_route__("/main", "GET", [](const HttpRequest& req, HttpResponse& res) {
            
            res.setContentType(ContentType::HTML);
            res.SendHtmlPage("main_page.html");
        });
        
        
        my_server->__add_route__("/get_user/{id}", "GET", [](const HttpRequest& req, HttpResponse& res) {
        
            std::string user_id = req.getParam("id");

            nlohmann::json user_response;
            if (user_id == "1") {
                user_response = {{"id", 1}, {"name", "John"}, {"email", "john@example.com"}};
            } else if (user_id == "2") {
                user_response = {{"id", 2}, {"name", "Jane"}, {"email", "jane@example.com"}};
            }

            res.setContentType(ContentType::JSON); 
            res.Send(user_response);
            
        });
        
        my_server->__add_route__("/save", "POST", [](const HttpRequest& req, HttpResponse& res) {
            
          
            nlohmann::json json_data = req.toJson();
            std::string name = json_data["name"].get<std::string>();
            
            nlohmann::json response = {
            {"message", "Data received successfully"},
            {"name", name} 
            };

            res.setStatusCode(201);
            res.setStatusMessage("Created"); 
            res.setContentType(ContentType::JSON);

            res.Send(response);
                  
        });
        
    
        my_server->start();



    } catch (const std::exception& e) {
        std::cerr << "Error starting the server: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
