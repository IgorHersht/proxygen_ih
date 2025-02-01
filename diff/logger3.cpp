#include <source_location>
#include <string_view>
#include <string>
#include <array>

template <auto SV> std::string log(const std::string_view message, const std::source_location location = std::source_location::current())
{
    std::string msg{SV}; msg += ":";
    msg += message; msg += "@";
    msg += location.file_name(); msg += ":";
    msg += location.function_name(); msg += ":";
    msg += std::to_string(location.line()); msg += ":";
    msg += std::to_string(location.column()); msg += ":";
    return msg;
}

template<std::size_t N> struct Str {
    char chars[N];
};
constexpr char INFO [6] {"INF"};
constexpr char ERR[6] {"ERROR"};
constexpr char WRN[6] {"WARN"};

// test
#include <iostream>
int main(int, char*[])
{
    std::cout << log<INFO>("XXXXX") << std::endl;
    std::cout << log<ERR>("XXXXX")<< std::endl;
}
/*
 *INF:XXXXX@/home/ihersht/projects/boost/main.cpp:int main(int, char**):28:27:
ERROR:XXXXX@/home/ihersht/projects/boost/main.cpp:int main(int, char**):29:26:
 */
