// filter_typle_by_type.cpp
#include <tuple>
#include <type_traits>
// decl
template<typename Tuple, template<typename> class Predicate> struct filter_tuple;
//empty specilization
template<template<typename> typename Predicate> struct filter_tuple<std::tuple<>, Predicate> { using type = std::tuple<>; };

template<typename T, typename... Ts, template<typename> class Predicate> class filter_tuple<std::tuple<T, Ts...>, Predicate> {
    using rest_filtered = typename filter_tuple<std::tuple<Ts...>, Predicate>::type;
public:
    using type = std::conditional_t< Predicate<T>::value, decltype(std::tuple_cat(std::declval<std::tuple<T>>(), std::declval<rest_filtered>())), rest_filtered >;
};

// test
template<typename T> using is_int = std::is_same<T, int>;// Predicate
using OriginalTuple = std::tuple<int, double, char, int>;
using FilteredTuple = typename filter_tuple<OriginalTuple, is_int>::type;
static_assert(std::is_same_v<FilteredTuple, std::tuple<int,int>>);

int main(){ }


