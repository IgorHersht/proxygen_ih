#include <string_view>
#include <bitset>
#include<type_traits>
#include <assert.h>
#include<string>
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

// for test on printable ascii
    std::string toString() const{
        std::string out;
        constexpr size_t size = sizeof(T) * 8;
        std::bitset<size> in(_value);
        std::bitset<8> bitsOut;

        for (size_t p = 0; p < size; ++p) {
            size_t index = p%OneElementBits;
            if(index == 0){
                const unsigned long ch = bitsOut.to_ulong();
                if((ch > 31) && (ch < 127)) {
                    out += char(ch);
                }
                bitsOut.reset();
            }
            bitsOut[index] = in[p];
        }
        return out;
    }

private:
    constexpr void init(std::string_view  value){
        const size_t size = value.size();
        if(size > MaxElementNum ){
            _value = _invalidValue;
            return ;
        }
        for (size_t p = 0; p < size; ++p) {
            _value |= (T(value[p]) << shifts[p]);
        }
    }
private:
    static constexpr T _invalidValue {std::numeric_limits<T>::max()};
    T _value{ };
};

template< typename T> using ByteIntegralValue = IntegralValueT<T, 0, 256>;
template< typename T> using AsciiIntegralValue = IntegralValueT<T, 0, 128>;
template< typename T> using AlphaNumericIntegralValue = IntegralValueT<T, 0, 62>;// need extra constexpr mapping
template< typename T> using NumericIntegralValue = IntegralValueT<T, size_t('0'), size_t('9') +1 >;// need extra constexpr mapping

//test

#include <assert.h>
#include <iostream>

void testInit();
void test0();
void test1();
void test2();


int main() {
    testInit();
    test0();
    test1();
    test2();

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

    const char* s0 = "1234";
    IntegralValueT<uint128_t, 0, 127> ascii0(s0);
    std::string s1 = ascii0.toString();
    assert(s0 == s1);
    int i =1;

}

// C string and std::string_view
constexpr std::string_view  ar31("ax");
constexpr std::string_view  ar32{ "abcd" };
int strTest1(std::string_view val) {

    switch (ByteIntegralValue<uint32_t>(val)) {
        case ByteIntegralValue<uint32_t>(ar31): {
            return 1;
        }
        case ByteIntegralValue<uint32_t>(ar32): {
            return 2;
        }
        default: {
            return 0;
        }
    }
}

// upto 7 chars
constexpr std::string_view  ar71{ "abc" };
constexpr std::string_view  ar72{ "abbcd" };
int strTest2(std::string_view val) {
    switch (ByteIntegralValue<uint64_t>(val)) {
        case ByteIntegralValue<uint64_t>(ar71): {
            return 1;
        }
        case ByteIntegralValue<uint64_t>(ar72): {
            return 2;
        }
        default: {
            return 0;
        }
    }
}

// std::array (initilized here with null terminatos => max size 15) and string_view
constexpr const char* ar151{ "0123456789abcde" };
constexpr std::string_view  ar152{ "0123456789abcdef" };
int strTest3(const char* val) {
    switch (ByteIntegralValue<uint128_t>(val)) {
        case ByteIntegralValue<uint128_t>(ar151): {
            return 1;
        }
        case ByteIntegralValue<uint128_t>(ar152): {
            return 2;
        }
        default: {
            return 0;
        }
    }
}


void test0(){
    //compile errors
    //constexpr IntegralValue<uint32_t> rv1(std::string_view ("12345"));
    //constexpr IntegralValue<uint32_t> rv2("12345");

    //invalid runtime
    //assert(IntegralValue<uint32_t>("1234") != IntegralValue<uint32_t>("12345"));
    //assert(IntegralValue<uint32_t>("12345") == IntegralValue<uint32_t>("12346"));
    //assert(IntegralValue<uint32_t>(std::string_view("1234")) != IntegralValue<uint32_t>(std::string_view("12345")));
    //assert(IntegralValue<uint32_t>(std::string_view("12345")) == IntegralValue<uint32_t>(std::string_view("12346")));


    assert(strTest1("ax") == 1);
    assert(strTest1("abcd") == 2);
    assert(strTest1("c") == 0);

    assert(strTest2("abc") == 1);
    assert(strTest2("abbcd") == 2);
    assert(strTest2("abbbbb") == 0);

    assert(strTest3("0123456789abcde") == 1);
    assert(strTest3("0123456789abcdef") == 2);
    assert(strTest3("0123456789ab") == 0);

}

void test1(){
    // digits test
    uint32_t v4_d1_r = IntegralValueT<uint32_t, '0', ':'>("1234");
    constexpr uint32_t v4_d1_b1_c = IntegralValueT<uint32_t, '0', ':'>("1234");
    assert( v4_d1_r == v4_d1_b1_c);
    IntegralValueT<uint32_t, '0', ':'> iv("1234") ;
    auto s4_d1_r = iv.toString();

    uint64_t v8_d1_r = IntegralValueT<uint64_t, '0', ':'>("12345678");
    constexpr uint64_t v8_d1_b1_c = IntegralValueT<uint64_t, '0', ':'>("12345678");
    assert( v8_d1_r == v8_d1_b1_c);

    uint128_t v16_d1_r = IntegralValueT<uint128_t, '0', ':'>("1234567812345678");
    constexpr uint128_t v16_d1_b1_c = IntegralValueT<uint128_t, '0', ':'>("1234567812345678");
    assert( v16_d1_r == v16_d1_b1_c);

    int i =1;
}

void test2(){
    const char* in_8_r = "12345678";
    constexpr const char* in_8_c = "12345678";
    uint32_t v4_d1_r = AsciiIntegralValue<uint32_t>(in_8_r);
    constexpr uint32_t v4_d1_c = AsciiIntegralValue<uint32_t>(in_8_c);
    assert( v4_d1_r == v4_d1_c);
    //constexpr uint32_t v4_d1_b_c = AsciiIntegralValue<uint32_t>("123456789"); // compile error
    try{
        uint32_t v4_d1_b_r = AsciiIntegralValue<uint32_t>("123456789"); // exception
    }catch(...){
        int i =1;
    }

    constexpr char m{15};
    const char* in_16_r = "mmmmmmmmmmmmmmmm";// 16 chars
    constexpr const char* in_16_c = "mmmmmmmmmmmmmmmm";
    uint64_t  v8_d1_r = AsciiIntegralValue<uint64_t>(in_16_r);
    constexpr uint64_t  v8_d1_c = AsciiIntegralValue<uint64_t>(in_16_c);
    uint64_t max = std::numeric_limits<uint64_t>::max();
    assert( v8_d1_r == v8_d1_c);
// in_16_r = 184 467 440 737 095 51613
// max =     184 467 440 737 095 51615
    const char* in_32_r = "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm";
    constexpr const char* in_32_c = "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm";
    uint128_t  v16_d1_r = AsciiIntegralValue<uint128_t>(in_32_r);
    constexpr uint128_t  v16_d1_c = AsciiIntegralValue<uint128_t>(in_32_c);
    assert( v8_d1_r == v8_d1_c);


    int i =1;

}

