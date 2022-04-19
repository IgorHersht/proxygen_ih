#include <string_view>
#include <array>
#include<type_traits>
#include<stdexcept>
#include "ConstexprMath.h"

#ifndef uint128_t
# define uint128_t  unsigned __int128
#endif

template< typename T, uint16_t begin, uint16_t end, bool isShortInputOk = true,  bool throwOnInvalid = true > class IntegralValueT {
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned int type");
    static_assert(end > begin);
    static constexpr size_t Base = end - begin;
    static constexpr size_t Size = sizeof(T);
    static constexpr ConstevalIntPows<Base, Size> Pows { };
public:
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
                arValue[p] = value[p] - begin;
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
            }

            for (size_t p = 0; p < size; ++p) {
                const int v =value[p ] - begin;
                _value += v * Pows._values[p];
            }
        }
    }

private:
    static constexpr T _invalidValue { isShortInputOk ? std::numeric_limits<T>::max() : T{} };
    T _value{  };
};

// This is  optimization (based on bitwise operations)  for begin = 0 and end = 256 and isShortInputOk = true, and bool throwOnInvalid = true
template< typename T> class IntegralValueT<T, 0, 256, true > {
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned int type");
    constexpr static  size_t Size = sizeof(T);
    constexpr static std::array byteShifts{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120 };
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
            if (size > Size) [[unlikely]] {
                throw std::runtime_error("Invalid input string");
            }
        }
        for (size_t p = 0; p < size; ++p) {
            _value |= (T(value[p]) << byteShifts[p]);
        }
    }
private:
    T _value{ };
};

template< typename T> using IntegralValue = IntegralValueT<T, 0, 256, true>;


/// test

#include <assert.h>
#include <iostream>
void test0();
void test1();

int main() {
    test0();
    test1();
    return 0;
}
// test0

// C string and std::string_view
constexpr std::string_view  ar31("ax");
constexpr std::string_view  ar32{ "abcd" };
int strTest1(std::string_view val) {

    switch (IntegralValue<uint32_t>(val)) {
        case IntegralValue<uint32_t>(ar31): {
            return 1;
        }
        case IntegralValue<uint32_t>(ar32): {
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
    switch (IntegralValue<uint64_t>(val)) {
        case IntegralValue<uint64_t>(ar71): {
            return 1;
        }
        case IntegralValue<uint64_t>(ar72): {
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
    switch (IntegralValue<uint128_t>(val)) {
        case IntegralValue<uint128_t>(ar151): {
            return 1;
        }
        case IntegralValue<uint128_t>(ar152): {
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
    assert( (IntegralValueT<uint32_t, '0', ':'>("1234") == 4321)); // should be in reverse order
    constexpr uint32_t v4_d1_b1_c = IntegralValueT<uint32_t, '0', ':'>("1234");
    assert( v4_d1_r == v4_d1_b1_c);

    //uint32_t v4_d1_r_b = IntegralValueT<uint32_t, '0', ':'>("12343"); // exception
    //constexpr uint32_t v4_d1_b1_c_b = IntegralValueT<uint32_t, '0', ':'>("12345"); // compile error

    uint32_t iv = IntegralValue<uint32_t>("1234");
    uint32_t iv2 = IntegralValueT<uint32_t, 0, 256>("1234");


    int i =1;
}


