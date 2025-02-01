#include <iostream>
#include <string>
#include <cctype>

std::string toUpperCase(std::string str) {
    for (char& c : str) {
        c = std::toupper(static_cast<unsigned char>(c));
    }
    return str;
}

std::string toLowerCase(std::string str) {
    for (char& c : str) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return str;
}

int main() {
    std::string original = "Hello, World!";
    std::string upper = toUpperCase(original);
    std::cout << upper << std::endl; // Outputs: HELLO, WORLD!

    std::string lower = toLowerCase(original);
    std::cout << lower << std::endl;

    return 0;
}
