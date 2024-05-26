// https://www.cppstories.com/2022/tuple-iteration-apply/
#include <tuple>
#include <concepts>

template <typename T>
struct IsTupleImpl : std::false_type {};

template <typename... U>
struct IsTupleImpl<std::tuple <U...>> : std::true_type {};

template <typename T> constexpr static bool is_tuple_v = IsTupleImpl<std::decay_t<T>>::value;

template <typename TupleT,  typename Fn>
void for_each(TupleT&& tp, Fn&& fn) requires is_tuple_v<TupleT>  {
    std::apply (
        [&fn]<typename ...T>(T&& ...args) requires (... && std::invocable<Fn, T>) {
            (std::forward<Fn>(fn)(std::forward<T>(args)), ...);
        }, std::forward<TupleT>(tp)
    );
}

template <typename TupleT, typename Out, typename Fn>
void for_each(TupleT&& tp, Out& out,  Fn&& fn) requires is_tuple_v<TupleT> {
    std::apply (
        [&fn, &out]<typename ...T>(T&& ...args)  requires (... && std::invocable<Fn, Out&, T>) {
            (std::forward<Fn>(fn)(out,std::forward<T>(args)), ...);
        }, std::forward<TupleT>(tp)
    );
}

/// test
#include <string>
#include <iostream>
#include <assert.h>
std::tuple<int, char> t1{ 10, 'x'};
std::tuple<int, char> t2{ 1000000, 'x'};

auto printElem1 = [](const auto& x) { std::cout << ", " << x; };

 struct printElem2 {
    template <typename T> void operator()(T t) {
        std::cout << ", " << t;
    }
};

auto check = [](bool& st, const auto& x) {
    std::string v = std::to_string(x);
    if( v.size() >5) {
        st = false;
    }
};


int main() {

    for_each(t1, printElem1);
    for_each(t1, printElem2());

    bool status = true;
    for_each(t1, status, check );
    assert(status);
    for_each(t2, status, check );
    assert(!status);
    int i =1;
}


