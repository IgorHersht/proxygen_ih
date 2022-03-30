
#include <cstring>
#include <string>
#include <string_view>
#include <array>
#include<cstddef>
#include<type_traits>
#include<algorithm>
#include <stdint.h>

#ifndef uint128_t
# define uint128_t  unsigned __int128
#endif


template<class> inline constexpr bool always_false_v = false;

template< typename T = char> struct ShortString {
    static_assert((std::is_integral_v<T> || std::is_same_v<T, uint128_t> || std::is_enum_v<T> ||
        std::is_same_v<T, unsigned char> || std::is_same_v<T, signed char> || std::is_same_v<T, bool>), "Should be an int type");

    constexpr static  size_t Size = sizeof(T);
    using BufferT = std::array<char, Size>;
    ShortString() = delete;
    constexpr  ShortString(char value) {
        _value[0] = value;
    }

    constexpr explicit ShortString(std::array<char, Size> value) :_value(value) {
    }

    constexpr explicit ShortString(const char* value) {// C string
        for (std::size_t i = 0; i < Size; i++) {
            if (Size == i) [[unlikely]] {
                exit(1);
            }
                if (value[i] == '\0') {
                    break;
                }
            _value[i] = value[i];
        }
    }

    constexpr explicit ShortString(std::string_view value) {
        if (value.size() > Size) [[unlikely]] {
            exit(1);
        }
        for (std::size_t i = 0; i < value.size(); i++) {
            _value[i] = value[i];
        }
    }

    constexpr operator T () const {
        if (std::is_constant_evaluated()) {
            if constexpr (Size == 1) {
                return _value[0];
            }
            else if constexpr (Size == 2) {
                return (T(_value[1]) << 8) + T(_value[0]);
            }
            else if constexpr (Size == 4) {
                return (T(_value[3]) << 24) + (T(_value[2]) << 16) + (T(_value[1]) << 8) + T(_value[0]);
            }
            else if constexpr (Size == 8) {
                return (T(_value[7]) << 56) + (T(_value[6]) << 48) + (T(_value[5]) << 40) + (T(_value[4]) << 32) +
                    (T(_value[3]) << 24) + (T(_value[2]) << 16) + (T(_value[1]) << 8) + T(_value[0]);
            }
            else if constexpr (Size == 16) {
                return (T(_value[15]) << 120) + (T(_value[14]) << 112) + (T(_value[13]) << 104) +
                    (T(_value[12]) << 96) +
                    (T(_value[11]) << 88) + (T(_value[10]) << 80) + (T(_value[9]) << 72) + (T(_value[8]) << 64) +
                    (T(_value[7]) << 56) + (T(_value[6]) << 48) + (T(_value[5]) << 40) + (T(_value[4]) << 32) +
                    (T(_value[3]) << 24) + (T(_value[2]) << 16) + (T(_value[1]) << 8) + T(_value[0]);
            }
            else {
                static_assert(always_false_v<T>, "Wrong type");
            }
        }
        return *(T*)(&_value);
    }

    constexpr std::string_view toStringView() const {
        return { &_value[0], _value.size() };
    }

    std::string toString() const { // just for debug
        if (std::find(_value.begin(), _value.end(), '\0') == _value.end()) {
            return std::string(_value.begin(), _value.end());
        }
        return std::string(&_value.at(0));
    }
protected:
    alignas(T)BufferT _value{};
};



/// test - usage
#include <assert.h>
#include <iostream>

// current impl one char only
constexpr char ch1 = 'a';
bool chatTest1(char val) {

    switch (val) {
    case ShortString(ch1): {
        return true;
    }
    default: {
        return false;
    }
    }
}
// char and std::string_view
constexpr char v1 = 'A';
constexpr std::string_view v2{ "ABCD" };
int chatTest2(std::string_view  val) {

    switch (ShortString<uint32_t>(val)) {
    case ShortString<uint32_t>(v1): {
        return 1;
    }
    case ShortString<uint32_t>(v2): {
        return 2;
    }
    default: {
        return 0;
    }
    }
}

// C string and std::string_view
constexpr const char* ar31("ax");
constexpr std::string_view  ar32{ "abcd" };
int strTest1(const char* val) {

    switch (ShortString<uint32_t>(val)) {
    case ShortString<uint32_t>(ar31): {
        return 1;
    }
    case ShortString<uint32_t>(ar32): {
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
int strTest2(const char* val) {
    switch (ShortString<uint64_t>(val)) {
    case ShortString<uint64_t>(ar71): {
        return 1;
    }
    case ShortString<uint64_t>(ar72): {
        return 2;
    }
    default: {
        return 0;
    }
    }
}

// std::array (initilized here with null terminatos => max size 15) and string_view
constexpr std::array<char, 16> ar151{ "0123456789abcde" };
constexpr std::string_view  ar152{ "0123456789abcdef" };
int strTest3(const char* val) {
    switch (ShortString<uint128_t>(val)) {
    case ShortString<uint128_t>(ar151): {
        return 1;
    }
    case ShortString<uint128_t>(ar152): {
        return 2;
    }
    default: {
        return 0;
    }
    }
}

int main(int argc, char* argv[])
{

    assert(chatTest1('a'));
    assert(!chatTest1('b'));

    assert(chatTest2("A") == 1);
    assert(chatTest2("ABCD") == 2);
    assert(chatTest2("C") == 0);

    assert(strTest1("ax") == 1);
    assert(strTest1("abcd") == 2);
    assert(strTest1("c") == 0);

    assert(strTest2("abc") == 1);
    assert(strTest2("abbcd") == 2);
    assert(strTest2("abbbbb") == 0);

    assert(strTest3("0123456789abcde") == 1);
    assert(strTest3("0123456789abcdef") == 2);
    assert(strTest3("0123456789ab") == 0);

    ShortString<uint64_t> str3(ar72);
    std::cout << str3.toString() << std::endl;
    std::cout << ShortString<uint128_t>("0123456789abcdef").toStringView() << std::endl;

    return 0;
}