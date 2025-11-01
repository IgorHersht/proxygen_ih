#include <tuple>
#include <memory>


template<template<class> class Wr, typename Tuple>  struct WrappedTupleElements;
template<template<class> class Wr, typename ... Els>  struct WrappedTupleElements <Wr, std::tuple<Els...> >{
    using type = std::tuple< Wr<Els>...>;
};
//test
#include <type_traits>
using T1 = std::tuple<int, double>;

using T2 = WrappedTupleElements<std::shared_ptr, T1>::type;
static_assert(std::is_same_v<std::tuple<std::shared_ptr<int>,std::shared_ptr<double> >, T2> );

int main()
{
}
