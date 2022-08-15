#include <cstdint>
#include <limits>
#include <cassert>
#include <array>
#include <charconv>

// create uint64_t from uint8_t and uint32_t with 24 bit between them
uint64_t combine( uint32_t v32, uint8_t v8 ){
    uint64_t out = v8;
    out |= (uint64_t(v32) << 32);
    return out;
}

// uint64_t 24 bytes in uint64_t
uint64_t insert24( uint64_t v64, uint32_t v24 ){
    constexpr uint32_t mask = std::numeric_limits<uint32_t>::max() & 0x00ffffff;
    v24 = v24 & mask;
    v64 |= (( uint64_t(v24) << 8))  ;
    return v64;
}

uint32_t extract24( uint64_t v64) {
    constexpr uint64_t mask = 0x00000000ffffff00;
    return ( (v64 & mask) >> 8);
}

uint64_t extract64( uint64_t v64) {
    constexpr uint64_t mask = 0xffffffff000000ff;
    return  (v64 & mask) ;
}

int main() {
    uint8_t v8 = 0xfd;
    uint32_t v24  = 0x00fffffb;
    uint32_t v32 = 0xfffffff7;

    uint64_t com1 = combine(v32, v8);
    uint64_t com1Test = 0xfffffff7000000fd;
    assert(com1 == com1Test);

    uint64_t com2 = insert24(com1, v24);
    uint64_t com2Test = 0xfffffff7fffffbfd;
    assert(com2 == com2Test);

    uint32_t v24Test = extract24(com2);
    assert(v24 == v24Test);

    uint64_t com1Test2 = extract64(com2);
    assert(com1 == com1Test2);

    int i =1;
}
