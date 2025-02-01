#include <iostream>
#include <tuple>
#include <type_traits>

// A trait to create a new tuple type from an existing tuple type
// from type trait TypeToType
// Base case empty tuple
template <template<typename> class TypeToType, typename Tuple>
struct TupleToTuple {
    using Type = std::tuple<>; // Base case: empty tuple
};

// Specialization for non-empty tuple
template <template<typename> class TypeToType, typename... Types>
struct TupleToTuple<TypeToType, std::tuple<Types...>> {
    using Type = std::tuple<typename TypeToType<Types>::Type...>;
};

//test
template <typename T> struct T2T { using Type = int; };
template <> struct T2T<double> { using Type = char; };

using OriginalTuple1 = std::tuple<short, double, std::string>;
using NewTuple1 = typename TupleToTuple<T2T,OriginalTuple1>::Type;
static_assert(std::is_same_v<std::tuple_element_t<0, NewTuple1>, int>);
static_assert(std::is_same_v<std::tuple_element_t<1, NewTuple1>, char>);
static_assert(std::is_same_v<std::tuple_element_t<2, NewTuple1>, int>);

using NewTupleEmpty = typename TupleToTuple<T2T,std::tuple<>>::Type;
static_assert(std::is_same_v<NewTupleEmpty, std::tuple<>>);

using OriginalTuple2 = std::tuple<float>;
using NewTuple2 = typename TupleToTuple<T2T,OriginalTuple2>::Type;
static_assert(std::is_same_v<std::tuple_element_t<0, NewTuple2>, int>);

// Example usage of the trait
int main() {



    return 0;
}
