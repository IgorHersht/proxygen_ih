#include <iostream>
#include <string_view>

template<auto Prefix>
class Logger {
    //...
public:
    void log(std::string_view msg) const {
        std::cout << Prefix << msg << '\n';
    }
};

template<std::size_t N>
struct Str {
    char chars[N];
    const char* value() {
        return chars;
    }
    friend std::ostream& operator<< (std::ostream& strm, const Str& s) {
        return strm << s.chars;
    }
};
template<std::size_t N> Str(const char(&)[N]) -> Str<N>;  // deduction guide

int main()
{
    Logger<Str{"> "}> logger;
    logger.log("hello");
}

