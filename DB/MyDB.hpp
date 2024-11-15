#include <iostream>
#include <unordered_map>
#include <string>

class MyDB {
private:
    std::unordered_map<std::string, std::string> data; 

    MyDB() {}

    MyDB(const MyDB&) = delete;
    MyDB& operator=(const MyDB&) = delete;

public:
   
       static MyDB& getInstance() {
        static MyDB instance;
        return instance;
    }


    bool addUser(const std::string& email, const std::string& password);
    bool verifyUser(const std::string& email, const std::string& password) const;
    void printUsers() const;

};


