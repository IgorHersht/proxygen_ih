#include <new>
#include <array>

#ifndef __cpp_lib_hardware_interference_size
namespace std {
    constexpr std::size_t hardware_constructive_interference_size = 64;
    constexpr std::size_t hardware_destructive_interference_size = 64;
}
#endif

template <std::size_t SZ> consteval std::size_t cacheLinePaddingSize() {
    constexpr std::size_t diff = std::hardware_destructive_interference_size -  SZ% std::hardware_destructive_interference_size;
    return diff ? diff : std::hardware_destructive_interference_size;
}

struct C {

    alignas(std::hardware_destructive_interference_size) std::array<std::byte, 312> _data;
    std::array<std::byte, cacheLinePaddingSize<312>()> _pad;
};




int main()
{
    auto x1 = 64 - 24%64;
    std::size_t s1  = cacheLinePaddingSize<24>();
    std::size_t s2  = cacheLinePaddingSize<64>();
    std::size_t s3  = cacheLinePaddingSize<129>();
    std::size_t s4  = cacheLinePaddingSize<192>();


    auto ss1 = sizeof(C);
    auto al = alignof(C);
    int i =1;

}

