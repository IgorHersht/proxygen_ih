#include <string_view>
#include <bitset>
#include<type_traits>
#include <assert.h>
#include<iostream>
#include "ConstexprMath.h"

#ifndef uint128_t
# define uint128_t  unsigned __int128
#endif


template<typename T, size_t Diff>
 consteval size_t bitNumber(){
    constexpr size_t MaxBits = sizeof(T) * 8;
    const std::bitset<MaxBits> bs(Diff);
    for(size_t i = MaxBits - 1; i >= 0; --i){
        if(bs[i] !=0){
            return i + 1;
        }
    }
    return 0;
}

template< typename T, size_t begin, size_t end> struct IntegralValueT {
    static_assert(((std::is_integral_v<T> || std::is_enum_v<T>) && std::is_unsigned_v<T>) ||
                  std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned int type");
    static_assert(end > begin);
    static constexpr size_t Base = end - begin;
    static constexpr size_t MaxBits = sizeof(T) * 8;
    static constexpr size_t OneElementBits = bitNumber<T, Base - 1>();
    static constexpr size_t MaxElementNum = MaxBits/OneElementBits;
    static constexpr std::array shifts = ConstevalIntSums<OneElementBits,sizeof(T) * 8>()._values;

    IntegralValueT() = delete;
    constexpr IntegralValueT(std::string_view  value) {
        init(value);
    }

    constexpr operator T () const {
        return _value;
    }

    constexpr  bool isValid(std::string_view  value) {
        return (_value != _invalidValue);
    }

private:
    constexpr void init(std::string_view  value){
        const size_t size = value.size();
        if(size > MaxElementNum ){
            return _invalidValue;
        }
        for (size_t p = 0; p < size; ++p) {
            _value |= (T(value[ size - p - 1  ]) << shifts[p]);
        }
    }
private:
    static constexpr T _invalidValue {std::numeric_limits<T>::max()};
    T _value{ };
};
void testInit();
int main() {
    testInit();

    return 0;
}

void testInit(){

    ConstevalIntPows<2u, 8u> pows { };
    ConstevalIntSums<8u, 128u> sums8 { };
    ConstevalIntSums<6u, 128u> sums6 { };
    ConstevalIntSums<4u, 128u> sums4 { };
    ConstevalIntSums<2u, 128u> sums2 { };
    size_t s = bitNumber<uint128_t, 255>();

    assert((IntegralValueT<uint64_t, 0, 256>::Base) ==256);
    assert((IntegralValueT<uint64_t, 0, 256>::OneElementBits) == 8);
    assert((IntegralValueT<uint64_t, 0, 256>::MaxElementNum) == 8 );
    std::array shifts256 = IntegralValueT<uint64_t, 0, 256>::shifts;

    assert((IntegralValueT<uint128_t, 0, 10>::Base) == 10);
    assert((IntegralValueT<uint128_t, 0, 10>::OneElementBits) == 4 );
    assert((IntegralValueT<uint128_t, 0, 10>::MaxElementNum) == 32);
    std::array shifts128 = IntegralValueT<uint64_t, 0, 128>::shifts;

    assert((IntegralValueT<uint128_t, 0, 63>::Base) == 63);
    assert((IntegralValueT<uint128_t, 0, 63>::OneElementBits) == 6);
    assert((IntegralValueT<uint128_t, 0, 63>::MaxElementNum) == 21);
    std::array shifts63 = IntegralValueT<uint128_t, 0, 63>::shifts;

    int i =1;

}
