#include <string_view>
#include <bitset>
#include <stdexcept>
#include<type_traits>
#include <assert.h>
#include<string>
#include<cstring>
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

template< typename T, size_t begin, size_t end, typename TranslationMap = NoTranslationMap> struct IntegralValueT {
    static_assert(((std::is_integral_v<T> || std::is_enum_v<T>) && std::is_unsigned_v<T>) ||
                  std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned int type");
    static_assert(end > begin);
    static constexpr size_t Base = end - begin;
    static constexpr std::array<size_t, Base> Range{};
    static constexpr size_t MaxBits = sizeof(T) * 8;
    static constexpr size_t OneElementBits = bitNumber<T, Base - 1>();
    static constexpr size_t MaxElementNum = MaxBits/OneElementBits;
    static constexpr std::array Shifts = ConstevalIntSums<OneElementBits, sizeof(T) * 8>()._values;
    static constexpr TranslationMap translationMap{};

    IntegralValueT() = delete;
    constexpr IntegralValueT(std::string_view  value) {
        init(value);
    }

    constexpr operator T () const {
        return _value;
    }

// printable ascii. Not fast  => for test only
    std::string toString() const{
        std::string out;
        constexpr size_t size = sizeof(T) * 8;
        std::bitset<8> bitsOut;
        std::bitset<size> in;
        if(size <= 64){
            in |= std::bitset<size>(_value);
        }else if( size == 128){//uint128_t
            uint64_t v1{}; uint64_t v2{};
            memcpy(&v1, &_value, 8);
            memcpy(&v2, (const char*)&_value + 8, 8);
            in |= std::bitset<size>(v1);
            in |= (std::bitset<size>(v2) <<64);
        }

        for (size_t p = 0; p < size; ++p) {
            size_t index = p%OneElementBits;
            if((p > 0) && (index == 0) ){
                 size_t ch = reverseElementTranslation(bitsOut.to_ulong());
                if((ch > 31) && (ch < 127)) {
                    out += ch;
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
        if(!std::is_constant_evaluated()) {
            if (size > MaxElementNum) [[unlikely]]{
                throw std::runtime_error("Input is too long ");// or set to an invalid
            }
            for (size_t p = 0; p < size; ++p) {
                const char v = elementTranslation(value[ p ]);
                if( (v < begin ) || (v >= end ) )[[unlikely]]{
                    throw std::runtime_error("Input out of Range"); // or set to an invalid
                }
                _value |= (T(v) << Shifts[p]);
            }
        } else{
            for (size_t p = 0; p < size; ++p) {
                const char v = elementTranslation(value[ p ]);
                Range[v - begin];// just to have compile error if out of Range
                _value |= (T(v) << Shifts[p]) ;
            }
        }
    }

    constexpr char elementTranslation(char ch) const{
        if constexpr(std::is_same_v<TranslationMap, AlphaNumericMap>){
            size_t tmp = translationMap._map[size_t(ch)];
            return translationMap._map[ch];
        }else{
            return ch;
        }
    }
    // Not fast  => for test only
     constexpr char reverseElementTranslation(char ch) const{
        if constexpr (std::is_same_v<TranslationMap, AlphaNumericMap>){
            if(ch == 0){
                return 0;
            }
            if((ch > 0) && ch <= 10){
                return ch   - elementTranslation('0') + '0';
            }
            if((ch > 10) && (ch < 37) ){
                return ch  - elementTranslation('A') + 'A';
            }
            if(ch == 37 ){
                return '_';
            }
            if((ch > 37) && (ch < 64) ){
                return ch - elementTranslation('a')  + 'a';
            }
            throw std::runtime_error("Input out of Range");

        }else{
            return ch;
        }
    }

private:
    T _value{ };
};

template< typename T> using ByteIntegralValue = IntegralValueT<T, 0, 256>;
template< typename T> using AsciiIntegralValue = IntegralValueT<T, 0, 128>;
template< typename T> using NumericIntegralValue = IntegralValueT<T, '0', '9' +1 >;// need extra constexpr mapping for alphaNumeric
 using AlphaNumericIntegralValue = IntegralValueT<uint128_t, 0, 64, AlphaNumericMap>;


//test

#include <assert.h>
#include <iostream>

void testAlphaNumeric(){

    int max = AlphaNumericIntegralValue::MaxElementNum ;
    constexpr const char* inc1 = "zzzzzzzzzzzzzzzzzzzzz";
    assert(strlen(inc1) == 21);
    constexpr AlphaNumericIntegralValue vc1(inc1);
    std::string inc1_t = vc1.toString();
    assert(inc1_t == inc1 );
    assert(AlphaNumericIntegralValue("1000").toString() == "1000");
    assert(AlphaNumericIntegralValue("001000").toString() == "001000");
    assert(AlphaNumericIntegralValue("").toString() == "");

    //zeroes at the beginning and end are significant
    assert(AlphaNumericIntegralValue("1000")  != AlphaNumericIntegralValue("100"));
    assert(AlphaNumericIntegralValue("0xxx")  != AlphaNumericIntegralValue("xxx"));
    assert(AlphaNumericIntegralValue("x0")  != AlphaNumericIntegralValue("x"));

}

void testInit();
void test0();
void test1();
void test2();


int main() {
    testAlphaNumeric();
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

    constexpr AlphaNumericMap anmap;
    for(size_t p = 33; p < AlphaNumericMap::Size; ++p){
        std::cout << char(p) << "->" << anmap._map[p] << " ";
    }

    assert((IntegralValueT<uint64_t, 0, 256>::Base) ==256);
    assert((IntegralValueT<uint64_t, 0, 256>::OneElementBits) == 8);
    assert((IntegralValueT<uint64_t, 0, 256>::MaxElementNum) == 8 );
    std::array shifts256 = IntegralValueT<uint64_t, 0, 256>::Shifts;

    assert((IntegralValueT<uint64_t, 0, 10>::MaxElementNum) == 16 );

    assert((IntegralValueT<uint128_t, 0, 10>::Base) == 10);
    assert((IntegralValueT<uint128_t, 0, 10>::OneElementBits) == 4 );
    assert((IntegralValueT<uint128_t, 0, 10>::MaxElementNum) == 32);
    std::array shifts128 = IntegralValueT<uint64_t, 0, 128>::Shifts;

    assert((IntegralValueT<uint128_t, 0, 64>::Base) == 64);
    assert((IntegralValueT<uint128_t, 0, 64>::OneElementBits) == 6);
    assert((IntegralValueT<uint128_t, 0, 64>::MaxElementNum) == 21);
    std::array shifts63 = IntegralValueT<uint128_t, 0, 64>::Shifts;

    const char* s0 = "123";
    IntegralValueT<uint128_t, 0, 127> ascii0(s0);


    const char* brin1 = "1233456789";
    try {
        uint64_t br1 = IntegralValueT<uint64_t, 0, 256>(brin1); // 'input is too long => exception
    }catch(...){
        int i = 1;
    }

    constexpr const char* bcin1 = "1233456789";
    //constexpr uint64_t bc1 = IntegralValueT<uint64_t, 0, 256>(bcin1); // 'input is too long => => compile error

    const char* brin2 = "123N";
    try {
        uint64_t br2 = IntegralValueT<uint64_t, 0, 63>(brin2); // 'N' out of Range => exception
    }catch(...){
        int i = 1;
    }

    constexpr const char* bcin2 = "123N";
    //constexpr uint64_t br1 = IntegralValueT<uint64_t, 0, 63>(bcin2); // 'N' out of Range => compile error
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
    size_t max32 = IntegralValueT<uint32_t, '0', ':'>::MaxElementNum;// 8
    uint32_t v4_d1_r = IntegralValueT<uint32_t, '0', ':'>("12345678");
    constexpr uint32_t v4_d1_b1_c = IntegralValueT<uint32_t, '0', ':'>("12345678");
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
    const char* in_8_r = "1234";
    constexpr const char* in_8_c = "1234";
    uint32_t v4_d1_r = AsciiIntegralValue<uint32_t>(in_8_r);
    constexpr uint32_t v4_d1_c = AsciiIntegralValue<uint32_t>(in_8_c);
    assert( v4_d1_r == v4_d1_c);
    //constexpr uint32_t v4_d1_b_c = AsciiIntegralValue<uint32_t>("12345"); // compile error
    try{
        uint32_t v4_d1_b_r = AsciiIntegralValue<uint32_t>("123456789"); // exception
    }catch(...){
        int i =1;
    }

    constexpr char m{15};
    const char* in_16_r = "mmmmmmmmm";
    constexpr const char* in_16_c = "mmmmmmmmm";
    uint64_t  v8_d1_r = AsciiIntegralValue<uint64_t>(in_16_r);
    constexpr uint64_t  v8_d1_c = AsciiIntegralValue<uint64_t>(in_16_c);
    uint64_t max = std::numeric_limits<uint64_t>::max();
    assert( v8_d1_r == v8_d1_c);
// in_16_r = 184 467 440 737 095 51613
// max =     184 467 440 737 095 51615
    const char* in_32_r = "mmmmmmmmmmmmmmmmmm";
    constexpr const char* in_32_c = "mmmmmmmmmmmmmmmmmm";
    uint128_t  v16_d1_r = AsciiIntegralValue<uint128_t>(in_32_r);
    constexpr uint128_t  v16_d1_c = AsciiIntegralValue<uint128_t>(in_32_c);
    assert( v8_d1_r == v8_d1_c);


    int i =1;

}
