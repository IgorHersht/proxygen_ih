#include <string_view>
#include <array>
#include<type_traits>
#include<stdexcept>
#include "ConstexprMath.h"

#ifndef uint128_t
# define uint128_t  unsigned __int128
#endif
// TODO increase max size of input string
//In current implementation we assume that every char in input string corresponds one byte in a integral value.
// It means max size of the input string is 16 ( sizeof(uint128_t) ).
//But if we can limit sizes of chars in input string we can try to compress.
//For example, if assume that input is limited to ASCII chars ( [0,127]  )
//we can try to fit two chars in one byte => max size of the input string is 32.

template< typename T, uint16_t begin, uint16_t end,  bool throwOnInvalid = true > class IntegralValueT {
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned int type");
    static_assert(end > begin);
    static constexpr size_t Base = end - begin;
    static constexpr size_t Size = maxPow<Base,0ul>();;
    static constexpr ConstevalIntPows<Base, Size> Pows { };
public:
    static constexpr T _invalidValue {  };
    IntegralValueT() = delete;
    constexpr explicit IntegralValueT(std::string_view  value) {
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

    constexpr bool isValid() const{
        return (_value != _invalidValue);
    }
private:
    constexpr void init(std::string_view  value) {
        const size_t size = value.size();
        if(size > Size) [[unlikely]] {
            _value = _invalidValue;
            return;;
        }
        for (size_t p = 0; p < size; ++p) {
            const int v =value[ size - p  -1 ] - begin;
            if( (v  < 0) || (v >= Base )  ) [[unlikely]] {
                _value = _invalidValue;
                return;
            }
            if((std::numeric_limits<T>::max() -v * Pows._values[p])   <= _value ) [[unlikely]]{
                _value = _invalidValue;
                return;
            }
            _value += v * Pows._values[p];
        }
    }

    T _value{  };
};

// This is  optimization (based on bitwise operations)  for begin = 0 and end = 256 and isShortInputOk = true, and bool throwOnInvalid = true
template< typename T> class IntegralValueT<T, 0, 256, true> {
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned int type");
    constexpr static  size_t Size = sizeof(T);
    constexpr static std::array shifts{0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120 };
public:
    IntegralValueT() = delete;
    constexpr IntegralValueT(std::string_view  value) {
        init(value);
    }

    constexpr operator T () const {
        return _value;
    }

    constexpr static bool isValid(std::string_view  value) {
        return (value.size() <= Size) ? true : false;
    }
private:
    constexpr void init(std::string_view  value) {
        const size_t size = value.size();
        if (!std::is_constant_evaluated()) {
            if (!isValid(value)) [[unlikely]] {
                throw std::runtime_error("Invalid input string");
            }
        }
        for (size_t p = 0; p < size; ++p) {
            _value |= (T(value[size - p -1 ]) << shifts[p]);
        }
    }

    T _value{ };
};

template< typename T> using ByteIntegralValue = IntegralValueT<T, 0, 256, true>;

// This is  optimization (based on bitwise operations)  for begin = 0 and end = 128 and isShortInputOk = true, and bool throwOnInvalid = true
template< typename T> class IntegralValueT<T, 0, 128, true> {
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned int type");
    constexpr static  size_t Size = sizeof(T);
    constexpr static std::array shifts{0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72,76,80,84,88,92,96,100,104,108,112, 116,120,124 };
public:
    IntegralValueT() = delete;
    constexpr IntegralValueT(std::string_view  value) {
        init(value);
    }

    constexpr operator T () const {
        return _value;
    }

    constexpr static bool isValid(std::string_view  value) {
        return (value.size()  <= Size * 2) ? true : false;
    }
private:
    constexpr void init(std::string_view  value) {
        const size_t size = value.size();
        if (!std::is_constant_evaluated()) {
            if (!isValid(value)) [[unlikely]] {
                throw std::runtime_error("Invalid input string");
            }
        }
        for (size_t p = 0; p < size; ++p) {
            _value |= (T(value[ size - p - 1  ]) << shifts[p]);
        }
    }
private:
    T _value{ };
};

template< typename T> using AsciiIntegralValue = IntegralValueT<T, 0, 128, true>;



/// test

#include <assert.h>
#include <iostream>
void test0();
void test1();
void test2();

int main() {
    test0();
    test1();
    test2();
    return 0;
}
// test0

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
    const char* v = "12345";
    uint32_t v4_d1_r = IntegralValueT<uint32_t, '0', ':'>(v);
    constexpr uint32_t  v4_d1_c = IntegralValueT<uint32_t, '0', ':'>("12345");
    assert( (v4_d1_c  == 12345));
    constexpr uint32_t v4_d1_b1_c = IntegralValueT<uint32_t, '0', ':'>("12345");
    static_assert(v4_d1_b1_c != IntegralValueT<uint32_t, '0', ':'>::_invalidValue );
    assert( v4_d1_r == v4_d1_b1_c);

    uint64_t v8_d1_r = IntegralValueT<uint64_t, '0', ':'>("123456789");
    assert( (IntegralValueT<uint64_t, '0', ':'>("123456789") == 123456789));

    uint128_t v16_d1_r = IntegralValueT<uint128_t, '0', ':'>("99999999999999999999");
    constexpr uint128_t v16_d1_b1_c = IntegralValueT<uint128_t, '0', ':'>("99999999999999999999");
    static_assert(v16_d1_b1_c != IntegralValueT<uint128_t, '0', ':'>::_invalidValue );
    assert( v16_d1_r == v16_d1_b1_c);


    uint32_t iv = ByteIntegralValue<uint32_t>("1234");
    uint32_t iv2 = IntegralValueT<uint32_t, 0, 256, false>("1234");
    constexpr uint32_t iv_c = ByteIntegralValue<uint32_t>("1234");
    constexpr uint32_t iv2_c = IntegralValueT<uint32_t, 0, 256,  false>("1234");
    assert( iv == iv2);
    assert( iv_c == iv2_c);
    assert( iv == iv_c);

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

    const char* in_16_r = "1234567812345678";
    constexpr const char* in_16_c = "1234567812345678";
    uint64_t  v8_d1_r = AsciiIntegralValue<uint64_t>(in_16_r);
    constexpr uint64_t  v8_d1_c = AsciiIntegralValue<uint64_t>(in_16_c);
    assert( v8_d1_r == v8_d1_c);

    constexpr char m{127};
    const char* in_32_r = "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm";
    constexpr const char* in_32_c = "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm";
    uint128_t  v16_d1_r = AsciiIntegralValue<uint128_t>(in_32_r);
    constexpr uint128_t  v16_d1_c = AsciiIntegralValue<uint128_t>(in_32_c);
    assert( v8_d1_r == v8_d1_c);


}



