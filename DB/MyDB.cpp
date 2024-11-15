 #include "MyDB.hpp"
 

bool MyDB::addUser(const std::string& email, const std::string& password) {
    // Verificăm dacă utilizatorul există deja
    if (data.find(email) != data.end()) {
        std::cout << "Utilizatorul există deja.\n";
        return false; // Email-ul există deja
    }

    // Adaugăm utilizatorul nou
    data[email] = password;
    std::cout << "Utilizator adăugat cu succes.\n";
    return true;
}

// Verifică dacă un utilizator există și dacă parola este corectă
bool MyDB::verifyUser(const std::string& email, const std::string& password) const {
    auto it = data.find(email);

    // Verificăm dacă email-ul există în "baza de date" și parola corespunde
    if (it != data.end() && it->second == password) {
        std::cout << "Autentificare reușită.\n";
        return true;
    }

    std::cout << "Autentificare eșuată. Email sau parolă incorecte.\n";
    return false;
}

// Opțional: metodă pentru afișarea tuturor utilizatorilor din "baza de date" (pentru debug)
void MyDB::printUsers() const {
    std::cout << "Utilizatori în baza de date:\n";
    for (const auto& [email, password] : data) {
        std::cout << "Email: " << email << ", Parolă: " << password << "\n";
    }
    }