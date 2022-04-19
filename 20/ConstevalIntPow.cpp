
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
    static_assert((std::numeric_limits<T>::max() / Base ) >= ConstevalIntPow<Base, Exp - 1>::_value, "Overflow in ConstevalIntPow ");
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
} ;

template <auto Base, std::size_t Size> struct ConstevalIntPows{
    using T = decltype(Base);
    static_assert( ( (std::is_integral_v<T>  || std::is_enum_v<T>  ) &&  std::is_unsigned_v<T> ) || std::is_same_v<T, unsigned char> || std::is_same_v<T, uint128_t>, "Should be a unsigned integer type");
    static_assert((Size > 0) && (Base >0 ), "Base and Size should be more than 0 ");
    static_assert(ConstevalIntPow<Base, Size - 1>::_isValid,  "Overflow in ConstevalIntPows " );
    consteval ConstevalIntPows(){
        _values[0]  = 1;
        if constexpr(Size > 1){
            for( std::size_t pos = 1; pos < _values.size(); ++pos ){
                _values[pos] = _values[pos -1] * Base ;
            }
        }
    }
    consteval operator std::array<T, Size> () const{
      return  _values;
    }
    std::array<T, Size> _values{};
};
void test0();
int main()
{
    test0();
    return 0;
}


void test0(){
    uint32_t l = std::numeric_limits<uint32_t>::max();

    constexpr uint32_t c20 = ConstevalIntPow<2u, 0>();
    constexpr uint32_t c21 = ConstevalIntPow<2u, 1>();
    constexpr uint32_t c2_31 = ConstevalIntPow<2u, 31>();
    // compile errors Overflow
    //constexpr uint32_t c2_32 = ConstevalIntPow<2u, 32>::_value;
   // constexpr uint32_t c2_50 = ConstevalIntPow<2u, 50>::_value;


    constexpr auto p2_1 = ConstevalIntPows<2u, 1>();
    constexpr auto p2_2 = ConstevalIntPows<2u, 2> ();
    constexpr std::array<uint32_t, 31> p2_31 = ConstevalIntPows<2u, 31> ();
    constexpr ConstevalIntPows<2u, 32> p2_32;;
//    // compile error Overflow
//      ConstevalIntPows<2u, 33> p2_33;


    int i = 1;



}
