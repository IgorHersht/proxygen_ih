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
    static_assert(ConstevalIntPow<Base, Size - 1>::_isValid,  "Overflow in ConstevalIntPows " );
    consteval ConstevalIntPows(){
        _values[0]  = 1;
        if constexpr(Size > 1) {
            for( std::size_t pos = 1; pos < _values.size(); ++pos ) {
                _values[pos] = _values[pos -1] * Base ;
            }
        }
    }
    consteval operator std::array<T, Size> () const {
        return  _values;
    }
    std::array<T, Size> _values {};
};

template <auto Base, std::size_t Exp> consteval  std::size_t maxPow(){
      if constexpr(ConstevalIntPow<Base, Exp>::_isValid ){
          uint128_t v = ConstevalIntPow<Base, Exp>::_value;
          return maxPow<Base, Exp + 1> ();
      }else{
          return Exp;
      }
}
#endif //TEST1_CONSTEXPRMATH_H

