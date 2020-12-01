#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <thread>
#include <type_traits>
#include <stdio.h>
#include <tuple>
#include <array>

constexpr std::tuple t{ 1,2.2 };//error in c++14
constexpr std::array a{ 1,2 };//error in c++14

int main()
{
    std::array a2{
            "xxxxxxx",
            "yyyyyy",

    };
    int i = a.size();

    return 0;
}
