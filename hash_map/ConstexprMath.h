//
// Created by ihersht on 4/14/22.
//

#ifndef TEST1_CONSTEXPRMATH_H
#define TEST1_CONSTEXPRMATH_H

#include <array>
#include<type_traits>
#include <cstdint>
#include <limits>

#ifndef uint128_t
# define uint128_t  unsigned __int128
#endif

template <typename T> constexpr bool isBitSet(T value, size_t  pos){
    return (value & (T(1)<<pos));
}

template <typename T> constexpr void setBit(T& value, size_t  pos){
    value = (value | (T(1) << pos));
}

template <typename T> constexpr void unsetBit(T& value, size_t  pos){
    value = ( value & ~(T(1) << pos));
}

template < auto Base, std::size_t Exp> struct ConstevalIntPow {
    using T = decltype(Base);
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned integral type");
    static constexpr bool _isValid = ((std::numeric_limits<T>::max() / Base ) >= ConstevalIntPow<Base, Exp - 1>::_value);
    consteval operator T () const{
        return  _value;
    }
    static constexpr T _value = ConstevalIntPow<Base, Exp - 1>::_value * Base;
} ;

template <  auto Base > struct ConstevalIntPow<Base, 0ul> {
    using T = decltype(Base);
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned integral type");
    static constexpr bool _isValid = true;
    consteval operator T () const{
        return  _value;
    }
    static constexpr T _value = 1;
};

template <auto Base, std::size_t Size> struct ConstevalIntPows {
    using T = decltype(Base);
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned integer type");
    static_assert((Size > 0) && (Base >0 ), "Base and Size should be more than 0 ");
    consteval ConstevalIntPows(){
        _values[0]  = 1;
        if constexpr(Size > 1) {
            for( std::size_t pos = 1; pos < _values.size(); ++pos ) {
                if( (std::numeric_limits<T>::max() / Base ) <_values[pos -1]){
                    _values =  _ivalidValues;
                    return;
                }
                _values[pos] = _values[pos -1] * Base ;
            }
        }
    }

    consteval operator std::array<T, Size> () const {
        return  _values;
    }
    consteval bool isValid() const {
        return  (_values == _ivalidValues) ;
    }
    constexpr static std::array<T, Size> _ivalidValues {};
    std::array<T, Size> _values {_ivalidValues };
};

template <auto Base, std::size_t Max> struct ConstevalIntSums {
    using T = decltype(Base);
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned integer type");
    static_assert( (Base >0) && (Max >= Base) , "Base and Size should be more than 0 ");
    constexpr static std::size_t Size = Max/Base;
    consteval ConstevalIntSums(){
        _values[0]  = 0;
        if constexpr(Size > 1) {
            for( std::size_t pos = 1; pos < _values.size(); ++pos ) {
                if( (std::numeric_limits<T>::max() - Base ) <_values[pos -1]){
                    _values =  _invalidValues;
                    return;
                }
                _values[pos] = _values[pos -1] + Base ;
            }
        }
    }

    consteval operator std::array<T, Size> () const {
        return  _values;
    }
    consteval bool isValid() const {
        return  (_values == _invalidValues) ;
    }
    constexpr static std::array<T, Size> _invalidValues {std::numeric_limits<T>::max()};
    std::array<T, Size> _values { };
};

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

struct NoTranslationMap{
    constexpr char translate(char ch) const {
        return ch;
    }
    constexpr char reverseTranslate(char ch) const {
        return ch;
    }
};
struct AlphaNumericMap{
    constexpr static  size_t Size = 128;
    constexpr static  size_t InvalidValue = 128;
    static constexpr std::string_view AlphaNumericCharset ="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-abcdefghijklmnopqrstuvwxyz";
    constexpr AlphaNumericMap(){
        size_t size = 0;
        for(size_t p = 0; p < Size; ++p){
            if( (p >= '0') && (p <= '9') ){
                const size_t index = p - '0' + 1;
                _map[p]  = index;
                _reverseMap[index] = p;
            }else if((p >= 'A') && (p <= 'Z')){
                const size_t index = p - 'A' + 11;
                _map[p]  = index;
                _reverseMap[index] = p;
            } else if(p == '-'){
                const size_t index = 37;
                _map[p]  = index;
                _reverseMap[index] = p;
            }
            else if((p >= 'a') && (p <= 'z')){
                const size_t index = p - 'a' + 38;
                _map[p]  = index;
                _reverseMap[index] = p;
            }else{
                _map[p] = InvalidValue;
            }
        }
    }
    constexpr char translate(char ch) const {
        return _map[ch];
    }
    constexpr char reverseTranslate(char ch) const {
        if((ch >= 0) && (ch <= AlphaNumericCharset.size() )){
            return _reverseMap[ch];
        }
        return InvalidValue;
    }

    std::array<size_t , Size> _map{};
    std::array<size_t , AlphaNumericCharset.size() +1 > _reverseMap{};
};



#endif //TEST1_CONSTEXPRMATH_H

