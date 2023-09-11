//
// Created by root on 6/8/22.
//

#pragma once

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
