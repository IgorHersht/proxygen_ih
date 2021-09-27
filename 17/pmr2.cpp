
#include <iostream>
#include <string>
#include <unordered_map>
#include <array>
#include <cstddef>  // for std::byte
#include <memory_resource>
#include "tracknew.h"
/*
 * relocate vector to heap when bigeer than byffer.
 */
int main()
{
    // use memory on the stack without fallback on the heap:
    std::array<std::byte, 20> buf;
    std::pmr::monotonic_buffer_resource pool{buf.data(), buf.size()};
    TrackNew::reset();
    // and allocate too much memory:
    std::pmr::vector<int> coll {&pool};
    for (int i=0; i<2; ++i) {
        coll.emplace_back(i);
        auto p = &coll[0];
        std::cout <<p<< std::endl;
    }
    TrackNew::status();
    for (int i=0; i<2; ++i) {
        coll.emplace_back(i);
        auto p = &coll[0];
        std::cout <<p<< std::endl;
    }
    TrackNew::status();
}
/*
 * 0x7ffdf34caa20
0x7ffdf34caa24
0 allocations for 0 bytes
0x7ff42cd17fc0
0x7ff42cd17fc0
1 allocations for 64 bytes
 */
