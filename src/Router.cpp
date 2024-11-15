#include "Router.hpp"
#include "ServerException.hpp"
#include "Serializer.hpp"
#include "Deserializer.hpp"
#include <iostream>

// void Router::route_request(const std::string &request_text, int client_socket) {   

//     HttpRequest request = Deserializer::deserialize_request(request_text);

//     std::string key = request.getMethod() + " " + request.getPath();  // Menținem cheia completă

//     HttpResponse response(client_socket);

    

//     for(const auto& [route_key, handler] : routes_) {   

//           if (route_key.find("{") == std::string::npos) { 
//             // Dacă ruta nu are parametri, facem potrivire directă cu cheia
//             if (route_key == key) {
//                 std::cout << "Static route matched: " << route_key << std::endl;
//                 handler(request, response); 
//                 return;
//             }
//         } else {
//             // Dacă ruta are parametri, aplicăm regex pentru extragerea lor
//             std::string regexPattern = convertToRegex(route_key);
//             std::regex route_regex(regexPattern);
//             std::smatch match;

//             if (std::regex_match(key, match, route_regex)) {
//                 std::cout << "Dynamic route matched: " << route_key << std::endl;
//                 add_params_to_request(route_key, request.getPath(), request);
//                 handler(request, response); 
//                 return;
//             }
//         }
//     }

// }

void Router::route_request(HttpRequest& request, HttpResponse& response) {  

    std::string key = request.getMethod() + " " + request.getPath();  // Menținem cheia completă
  
    for(const auto& [route_key, handler] : routes_) {   

          if (route_key.find("{") == std::string::npos) { 
            // Dacă ruta nu are parametri, facem potrivire directă cu cheia
            if (route_key == key) {
                std::cout << "Static route matched: " << route_key << std::endl;
                handler(request, response); 
                return;
            }
        } else {
            // Dacă ruta are parametri, aplicăm regex pentru extragerea lor
            std::string regexPattern = convertToRegex(route_key);
            std::regex route_regex(regexPattern);
            std::smatch match;

            if (std::regex_match(key, match, route_regex)) {
                std::cout << "Dynamic route matched: " << route_key << std::endl;
                add_params_to_request(route_key, request.getPath(), request);
                handler(request, response); 
                return;
            }
        }
    }

}

void Router::add_params_to_request(const std::string &route_pattern, const std::string &url, HttpRequest& request) {
    
    std::string path_only = route_pattern.substr(route_pattern.find(' ') + 1);
    std::string regexPattern = convertToRegex(path_only);

    std::regex route_regex(regexPattern);
    std::smatch match;

    if (std::regex_match(url, match, route_regex)) {

        std::regex param_regex("\\{(\\w+)\\}");
        auto param_it = std::sregex_iterator(route_pattern.begin(), route_pattern.end(), param_regex);
        int i = 1;

        for (; param_it != std::sregex_iterator(); ++param_it, ++i) {
            std::string param_name = (*param_it)[1].str();  // Numele parametrului între acolade
            std::string param_value = match[i].str();       // Valoarea extrasă din URL

            request.setParam(param_name, param_value);

        }
    } 
}

std::string Router::convertToRegex(const std::string& route) {
    std::string regexRoute = route;
   
    regexRoute = std::regex_replace(regexRoute, std::regex("\\{\\w+\\}"), "([^/]+)");
    regexRoute = std::regex_replace(regexRoute, std::regex("\\."), "\\.");

    // Prefixăm regex-ul pentru a se potrivi de la început până la sfârșit, inclusiv metoda
    std::string finalRegex = "^" + regexRoute + "$";

    return finalRegex;
}
