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

struct NoTranslationMap{};
struct AlphaNumericMap{
    constexpr static  size_t Size = 128;
    constexpr static  size_t InvalidValue = 128;
    constexpr AlphaNumericMap(){
        size_t size = 0;
        for(size_t p = 0; p < Size; ++p){
            if( (p >= '0') && (p <= '9') ){
                _map[p]  = p - '0' + 1;
            }else if((p >= 'A') && (p <= 'Z')){
                _map[p]  = p - 'A' + 11 ;
            }else if(p == '-'){
                _map[p] = 37;
            }else if((p >= 'a') && (p <= 'z')){
             _map[p]  = p - 'a' + 38;
            }else{
                _map[p] = InvalidValue;
            }
        }

    }

    std::array<size_t , Size> _map{};
};



#endif //TEST1_CONSTEXPRMATH_H

