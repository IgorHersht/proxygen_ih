
#include <stacktrace>
#include <string>
#include <iostream>

#include <exception>

//! in cmake
// link_libraries(
//         stdc++exp
// )

std::string foo() {
    std::string tr = std::to_string(std::stacktrace::current());
    return tr;
}

struct StacktraceException : public std::runtime_error {
    explicit StacktraceException(const std::string &error ):
        std::runtime_error(error){
        _error = std::string(" exception: ") + std::runtime_error::what() + "\n";
        _error +=  std::to_string(std::stacktrace::current());
    }

    const char* what() const noexcept override {
        return _error.c_str();
    }

private:
    std::string _error;
};

void foo2() {
    throw StacktraceException("my exception");
}

int main()
{
   // std::cout << foo() << '\n';

    try {
        foo2();
    } catch (const StacktraceException& ex) {
        std::cout <<  ex.what()<< std::endl;
    }
}
