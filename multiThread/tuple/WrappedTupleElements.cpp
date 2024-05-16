#include <tuple>
#include <memory>

template<template<class> class Wr, typename Tuple>  struct WrappedTupleElements;
template<template<class> class Wr, typename ... Els>  struct WrappedTupleElements <Wr, std::tuple<Els...> >{
    using type = std::tuple< Wr<Els>...>;
};

using T1 = std::tuple<int, double>;

using T2 = WrappedTupleElements<std::shared_ptr, T1>::type;

int main()
{
T2   t2;
int i =1;
//...
}

