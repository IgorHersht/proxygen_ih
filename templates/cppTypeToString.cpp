#include <cxxabi.h>
#include <iostream>
#include <typeinfo>
#include <string>

template <typename Class> std::string cppTypeToString() {
    int status{1};
    std::string name = "unknown_name";
    const std::type_info& ti = typeid(Class);
    char* realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
    if (status == 0) {
        name = realname;
        std::free(realname);
    }
    return name;
}
//test
class MyClass {
public:
    void myFunction() {}
};

int main() {
    std::cout << cppTypeToString<MyClass>() <<std::endl;
    std::cout << cppTypeToString<int8_t>() <<std::endl;
    std::cout << cppTypeToString<double>() <<std::endl;

    return 0;
}
