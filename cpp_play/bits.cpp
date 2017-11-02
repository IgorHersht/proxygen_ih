
#include <iostream>
#include <stdlib.h>
#include <type_traits>



constexpr bool isBitSet(unsigned long x, unsigned  n){
	return (x & (1<<n));
}
constexpr bool isOdd(unsigned long x){
	return (x & 1);
}

constexpr unsigned long setBit(unsigned long x, unsigned  n){
	return (x | (1<<n));
}

constexpr unsigned long unsetBit(unsigned long x, unsigned  n){
	return ( x & ~(1<<n));
}

constexpr bool isPowerOf2(unsigned long n){
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

