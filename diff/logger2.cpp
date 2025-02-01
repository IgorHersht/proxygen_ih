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

using Severity = char[4];
constexpr Severity INFO {"INF"};
constexpr Severity ERR {"ERR"};
constexpr Severity WRN {"WRN"};

// test
#include <iostream>
int main(int, char*[])
{
    std::cout << log<INFO>("XXXXX");
}
/*
 * INF:XXXXX@/home/ihersht/projects/boost/main.cpp:int main(int, char**):26:25:
 */
