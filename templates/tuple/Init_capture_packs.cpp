#include <string>
#include <iostream>
template<std::convertible_to<std::string> ...T> auto make_prefixer(T&& ...args){
    using namespace std::string_literals;
    return [...p=std::string(std::forward<T>(args))](std::string msg) {
        // binary right fold over +
        return ((p + ": "s + msg + "\n"s) + ... + ""s);
    };
}

int main(){
    auto p = make_prefixer("BEGIN", "END");
    std::cout << p("message");
    // prints:
    // BEGIN: message
    // END: message
}

