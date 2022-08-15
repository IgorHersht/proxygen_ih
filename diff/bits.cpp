#include <cstdint>
#include <limits>
#include <cassert>
#include <array>


#include <iostream>
#include <stdlib.h>
#include <type_traits>

 bool is_little_endian(void)
{
    union {
        uint16_t i;
        char c[2];
    } bint = { 1 };

    return bint.c[0] == 1;
}

constexpr bool equal(uint64_t x, uint64_t  n){
    return !(x^n);
}

constexpr bool isBitSet(uint64_t x, uint64_t  n){
    return (x & (uint64_t(1)<<n));
}
constexpr bool isOdd(uint64_t x){
    return (x & uint64_t(1));
}

constexpr uint64_t setBit(uint64_t x, uint64_t  n){
    return (x | (uint64_t(1)<<n));
}

constexpr uint64_t unsetBit(uint64_t x, uint64_t  n){
    return ( x & ~(uint64_t(1)<<n));
}
// isPowerOf2 if n > 1 then only one bit set
//  pow(2,N) - 1 = 0111....1
constexpr bool isPowerOf2(uint64_t n){
    return ( n && (!(n & (n -1))) );
}


/////////////////////////////////////////////////////
int lookup[256]={0};
void setLookup(){
    for(int i = 0; i < 256; ++i){
        int v = 0;
        for(int j = 0; j < 8; ++j){
            if(isBitSet(i,j)){
                v++;
            }
            lookup[i] = v;
        }
    }
}

int numOfSetBits(int n)
{
    // print lookup table (number of bits set for integer i)
    // for (int i = 0; i < 256; i++)
    //    cout << i << " has " << lookup[i] << " bits\n";

    // assuming 32-bit(4 byte) integer, break the integer into 8-bit chunks
    // Note mask used 0xff is 11111111 in binary

    int count = lookup[n & 0xff] +    // consider first 8 bits
                lookup[(n >> 8) & 0xff] +     // consider next 8 bits
                lookup[(n >> 16) & 0xff] +    // consider next 8 bits
                lookup[(n >> 24) & 0xff];    // consider last 8 bits

    return count;
}

#include <bitset>

int main()
{
    setLookup();
    for(int i = 256; i < 500; i++){
        int v1 = numOfSetBits(i);
        int v2 = std::bitset<32>(i).count();
        if(v1 != v2){
            std::cout << 10 << " error" << std::bitset<32>(i) << std::endl;
        }
    }



    return 0;

}


