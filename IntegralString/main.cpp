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

template< typename T, uint16_t begin, uint16_t end, bool isShortInputOk = true,  bool throwOnInvalid = true > class IntegralValueTMS {
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned int type");
    static_assert(end > begin);
    static constexpr size_t Base = end - begin;
    static constexpr size_t Size = sizeof(T);
    static constexpr ConstevalIntPows<Base, Size> Pows { };
public:
    IntegralValueTMS() = delete;
    constexpr explicit IntegralValueTMS(std::string_view  value) {
        init(value);
        if (!std::is_constant_evaluated()) {
            if ((_value == _invalidValue) && throwOnInvalid) {
                throw std::runtime_error("Invalid input string");
            }
        }
    }

    constexpr operator T () const {
        return _value;
    }

    constexpr static bool isValid(std::string_view  value ) {
        const size_t size = value.size();
        if(size > Size) [[unlikely]] {
            return false;
        }
        if( (size < Size) && !isShortInputOk  ) [[unlikely]] {
            return false;
        }
        for (size_t p = 0; p < size; ++p) {
            const int v =value[p ] - begin;
            if( (v  < 0) || (v >= Base )  ) [[unlikely]] {
                return false;
            }
        }
        return true;
    }

private:
    constexpr void init(std::string_view  value) {
        const size_t size = value.size();

        if (std::is_constant_evaluated()) {
            std::array<char, Size> arValue { };
            for (size_t p = 0; p < size; ++p) {
                arValue[p] = value[size - p -1] - begin;
            }
            for (size_t p = 0; p < size; ++p) {
                const int v = arValue[p] ;
                if( (v  < 0) || (v >= Base )  ) {
                    _value = _invalidValue;
                    return;
                }
                _value += v * Pows._values[p];
            }
        }else{
            if(! isValid(value) ) [[unlikely]] {
                _value = _invalidValue;
                return;
            }

            for (size_t p = 0; p < size; ++p) {
                const int v =value[ size - p  -1 ] - begin;
                _value += v * Pows._values[p];
            }
        }
    }

private:
    static constexpr T _invalidValue { isShortInputOk ? std::numeric_limits<T>::max() : T{} };
    T _value{  };
};


 enum class ErrorPolicy{MaxInvalid, Exception};
template< typename T, size_t begin, size_t end, typename TranslationMap = NoTranslationMap, ErrorPolicy errorPolicy = ErrorPolicy::MaxInvalid  > struct IntegralValueTBit {
    static_assert(((std::is_integral_v<T> || std::is_enum_v<T>) && std::is_unsigned_v<T>) ||
                  std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned int type");
    static_assert(end > begin);
    static constexpr size_t Base = end - begin;
    static constexpr std::array<size_t, Base> Range{};
    static constexpr size_t MaxBits = sizeof(T) * 8;
    static constexpr size_t OneElementBits = bitNumber<T, Base - 1>();
    static constexpr size_t MaxElementNum = MaxBits/OneElementBits;
    static constexpr std::array Shifts = ConstevalIntSums<OneElementBits, sizeof(T) * 8>()._values;
    static constexpr T InvalidValue {std::numeric_limits<T>::max()};// assume max is invalid
    static constexpr TranslationMap translationMap{};

    IntegralValueTBit() = delete;
    constexpr IntegralValueTBit(std::string_view  value) {
        init(value);
    }

    constexpr bool isValid() const{
        return (_value != InvalidValue);
    }

    constexpr operator T () const {
        return _value;
    }

    constexpr std::array<char, MaxElementNum> toArray() const {
        std::array<char, MaxElementNum> out{};
        for (size_t p = 0; p < Shifts.size(); ++p){
            char in  = (char)(_value >> Shifts[p]) & OneElemenMask ;
            char ch = reverseElementTranslation(in);
            out.at( p ) = ch;
        }
        return out;
    }
// Not fast  => for test only
    std::string toString() const{
        const auto ar = toArray();
        std::string out(&ar[0], ar.size());
        out = out.c_str(); // to eliminate trailing '\0'
        return out;
    }
private:
    constexpr void init(std::string_view  value){
        const size_t size = value.size();
        if(!std::is_constant_evaluated()) {
            if (size > MaxElementNum) [[unlikely]]{
                if(errorPolicy == ErrorPolicy::MaxInvalid){
                    _value = InvalidValue;
                    return ;
                }else{
                    throw std::runtime_error("Input is too long ");
                }
            }
            for (size_t p = 0; p < size; ++p) {
                const char v = elementTranslation(value[ p ]);
                if( (v < begin ) || (v >= end ) )[[unlikely]]{
                    if(errorPolicy == ErrorPolicy::MaxInvalid){
                        _value = InvalidValue;
                        return ;
                    }else{
                        throw std::runtime_error("Input is out of range");
                    }
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
        return translationMap.translate(ch);
    }
    // printable ascii. Not fast  => for test only
     constexpr char reverseElementTranslation(char ch) const{
        return translationMap.reverseTranslate(ch);
    }

    static constexpr char getOneElemenMask(){
        char mask = 0;
        for(size_t p =0; p < OneElementBits; ++p ){
            setBit(mask, p);
        }
        return mask;
    }
private:
    static constexpr char OneElemenMask{getOneElemenMask()};
    T _value{ };
};

template< typename T> using ByteIntegralValue = IntegralValueTBit<T, 0, 256>;
template< typename T> using AsciiIntegralValue = IntegralValueTBit<T, 0, 128>;
 using AlphaNumericIntegralValue = IntegralValueTBit<uint128_t, 0, 64, AlphaNumericMap>;

//test
#include <assert.h>
#include <iostream>
#include "gen_string.h"

void testAlphaNumeric(){

    int max = AlphaNumericIntegralValue::MaxElementNum ;
    assert(max == 21);

    constexpr const char* inc1 = "zzzzzzzzzzzzzzzzzzzzz";
    assert(strlen(inc1) == 21);
    constexpr AlphaNumericIntegralValue vc1(inc1);
    std::string inc1_t = vc1.toString();
    assert(inc1_t == inc1 );

    constexpr const char* inc2 = "-zzzzzzzzz-zzzzzzzzz-";
    assert(strlen(inc2) == 21);
    constexpr AlphaNumericIntegralValue vc2(inc2);
    std::string inc2_t = vc2.toString();
   assert(inc2_t == inc2);


    assert(AlphaNumericIntegralValue("1-00").toString() == "1-00");
    assert(AlphaNumericIntegralValue("0010-0").toString() == "0010-0");
    assert(AlphaNumericIntegralValue("").toString() == "");

    //zeroes at the beginning and end are significant
    assert(AlphaNumericIntegralValue("1000")  != AlphaNumericIntegralValue("100"));
    assert(AlphaNumericIntegralValue("0xxx")  != AlphaNumericIntegralValue("xxx"));
    assert(AlphaNumericIntegralValue("x0")  != AlphaNumericIntegralValue("x"));

    // test on 1,000,000 random strings
    gen_strings(20);
    for(const std::string& s1: inputs){
      AlphaNumericIntegralValue v((s1.c_str()));
        std::string s2 = v.toString();
        assert(s1 == s2);
        if(s1.find('-') != std::string::npos){
            int i =1;
        }
    }
    gen_strings(17);
    for(const std::string& s1: inputs){
        std::string s2 = AlphaNumericIntegralValue(s1.c_str()).toString();
        assert(s1 == s2);
    }
    gen_strings(7);
    for(const std::string& s1: inputs){
        std::string s2 = AlphaNumericIntegralValue(s1.c_str()).toString();
        assert(s1 == s2);
    }

    // negative test
    {
        constexpr const char* inc1 = "0000000000000000000000";// too long
        assert(strlen(inc1) == 22);
        //constexpr AlphaNumericIntegralValue vc1(inc1); // compile error
        constexpr const char* inc2 = "012345$7890123456789";// wrong char $
        assert(strlen(inc2) == 20);
        //constexpr AlphaNumericIntegralValue vc2(inc2); // compile error

        const char* inr1 = "0000000000000000000000";// too long
        assert(strlen(inr1) == 22);
        assert( !AlphaNumericIntegralValue(inr1).isValid()); // invalid in runtime
        const char* inr2 = "012345$7890123456789";// wrong char $
        assert(strlen(inr2) == 20);
        assert( !AlphaNumericIntegralValue(inr2).isValid()); // invalid in runtime
        // all invalids are equal
        assert(AlphaNumericIntegralValue(inr1) == AlphaNumericIntegralValue(inr2));
    }

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
    for(size_t p = 32; p < AlphaNumericMap::Size -1; ++p){
        std::cout << char(p) << "->" << anmap._map[p] << " ";
    }

    assert((IntegralValueTBit<uint64_t, 0, 256>::Base) == 256);
    assert((IntegralValueTBit<uint64_t, 0, 256>::OneElementBits) == 8);
    assert((IntegralValueTBit<uint64_t, 0, 256>::MaxElementNum) == 8 );
    std::array shifts256 = IntegralValueTBit<uint64_t, 0, 256>::Shifts;

    assert((IntegralValueTBit<uint64_t, 0, 10>::MaxElementNum) == 16 );

    assert((IntegralValueTBit<uint128_t, 0, 10>::Base) == 10);
    assert((IntegralValueTBit<uint128_t, 0, 10>::OneElementBits) == 4 );
    assert((IntegralValueTBit<uint128_t, 0, 10>::MaxElementNum) == 32);
    std::array shifts128 = IntegralValueTBit<uint64_t, 0, 128>::Shifts;

    assert((IntegralValueTBit<uint128_t, 0, 64>::Base) == 64);
    assert((IntegralValueTBit<uint128_t, 0, 64>::OneElementBits) == 6);
    assert((IntegralValueTBit<uint128_t, 0, 64>::MaxElementNum) == 21);
    std::array shifts63 = IntegralValueTBit<uint128_t, 0, 64>::Shifts;

    const char* s0 = "123";
    IntegralValueTBit<uint128_t, 0, 127> ascii0(s0);


    const char* brin1 = "1233456789";
    try {
        uint64_t br1 = IntegralValueTBit<uint64_t, 0, 256>(brin1); // 'input is too long => exception
    }catch(...){
        int i = 1;
    }

    constexpr const char* bcin1 = "1233456789";
    //constexpr uint64_t bc1 = IntegralValueTBit<uint64_t, 0, 256>(bcin1); // 'input is too long => => compile error

    const char* brin2 = "123N";
    try {
        uint64_t br2 = IntegralValueTBit<uint64_t, 0, 63>(brin2); // 'N' out of Range => exception
    }catch(...){
        int i = 1;
    }

    constexpr const char* bcin2 = "123N";
    //constexpr uint64_t br1 = IntegralValueTBit<uint64_t, 0, 63>(bcin2); // 'N' out of Range => compile error
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
    size_t max32 = IntegralValueTBit<uint32_t, '0', ':'>::MaxElementNum;// 8
    uint32_t v4_d1_r = IntegralValueTBit<uint32_t, '0', ':'>("12345678");
    constexpr uint32_t v4_d1_b1_c = IntegralValueTBit<uint32_t, '0', ':'>("12345678");
    assert( v4_d1_r == v4_d1_b1_c);
    IntegralValueTBit<uint32_t, '0', ':'> iv("1234") ;
    auto s4_d1_r = iv.toString();

    uint64_t v8_d1_r = IntegralValueTBit<uint64_t, '0', ':'>("12345678");
    constexpr uint64_t v8_d1_b1_c = IntegralValueTBit<uint64_t, '0', ':'>("12345678");
    assert( v8_d1_r == v8_d1_b1_c);

    uint128_t v16_d1_r = IntegralValueTBit<uint128_t, '0', ':'>("1234567812345678");
    constexpr uint128_t v16_d1_b1_c = IntegralValueTBit<uint128_t, '0', ':'>("1234567812345678");
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
