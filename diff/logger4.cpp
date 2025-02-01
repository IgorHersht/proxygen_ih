#include <source_location>
#include <string_view>
#include <string>
#include <array>

template <auto SV> std::string log(const std::string_view message, const std::source_location location = std::source_location::current())
{
    std::string msg{SV.begin(), SV.end()}; msg += ":";
    msg += message; msg += "@";
    msg += location.file_name(); msg += ":";
    msg += location.function_name(); msg += ":";
    msg += std::to_string(location.line()); msg += ":";
    msg += std::to_string(location.column()); msg += ":";
    return msg;
}

template<std::size_t N> struct Str {
    std::array <char, N> chars;
};
constexpr std::array INFO {'I','N','F'};
constexpr std::array ERR {'E','R','R','O','R'};


// test
#include <iostream>
int main(int, char*[])
{
    std::cout << log<INFO>("XXXXX") << std::endl;
    std::cout << log<ERR>("XXXXX") << std::endl;

}
/*
 * INF:XXXXX@/home/ihersht/projects/boost/main.cpp:int main(int, char**):30:25:
ERROR:XXXXX@/home/ihersht/projects/boost/main.cpp:int main(int, char**):31:26:
 */
