#include <iostream>
#include <tuple>
#include <type_traits>



// A trait to create a new tuple type from an existing tuple type
// from type trait TypeToType
// Base case empty tuple
template < typename Tuple>
struct TupleToTuple {
    using Type = std::tuple<>;
    constexpr static Type _default{};
};

template <typename T> concept has_subtype = requires {typename T::Type;};
template <typename T> concept has_default = requires {T::_default;};
// Specialization for non-empty tuple
template < typename... Types> requires ((has_subtype<Types> && ...) && (has_default<Types> && ...) && (sizeof...(Types) > 0 ) )
struct TupleToTuple<std::tuple<Types...>>  {
    using Type = std::tuple<typename Types::Type...>;
    constexpr static Type _default{Types::_default...};
};
//test
template <int i> struct field;
template <> struct field<1> {
    using Type = int;
    constexpr static Type _default = 2;
};
template <> struct field<2> {
    using Type = char;
    constexpr static Type _default = 'c';
};

using Fields = std::tuple<field<1>, field<2> >;
using FieldsTypes = std::tuple<field<1>::Type, field<2>::Type >;

using FieldsTypes2 = typename TupleToTuple<Fields>::Type;
static_assert(std::is_same_v<FieldsTypes2, FieldsTypes>);

constexpr static FieldsTypes2 _default{TupleToTuple<Fields>::_default};
static_assert(std::get<0>(_default) == 2);
static_assert(std::get<1>(_default) == 'c');

int main() {
    FieldsTypes2 f;
    FieldsTypes2 d = _default;
    int i =1;

    return 0;
}
