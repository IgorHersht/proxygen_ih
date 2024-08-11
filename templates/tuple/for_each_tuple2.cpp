
#// https://www.cppstories.com/2022/tuple-iteration-apply/
#include <tuple>
#include <concepts>

template <typename T>
struct IsTupleImpl : std::false_type {};

template <typename... U>
struct IsTupleImpl<std::tuple <U...>> : std::true_type {};

template <typename T> constexpr static bool is_tuple_v = IsTupleImpl<std::decay_t<T>>::value;

template <typename T> concept TupleType = is_tuple_v<T>;


template <TupleType TupleT,  typename Fn>
void for_each(TupleT&& tp, Fn&& fn)  {
    std::apply (
        [&fn]<typename ...T>(T&& ...args) requires (... && std::invocable<Fn, T>) {
            (std::forward<Fn>(fn)(std::forward<T>(args)), ...);
        }, std::forward<TupleT>(tp)
    );
}

/// test
#include <string>
#include <iostream>
#include <assert.h>

auto increment1 = []( auto& t) { ++t ; };

struct increment2 {
    void operator()(auto& t) { ++t;}
};
auto increment3 = []<typename T> (const T* ) {
    std::cout << sizeof(T) <<std::endl;
};



int main() {
    std::tuple<int, char> t1{ 10, 'a'};

    for_each(t1, increment1);
    assert(std::get<0>(t1) == 11);
    assert(std::get<1>(t1) == 'b');
    for_each(t1, increment2());
    assert(std::get<0>(t1) == 12);
    assert(std::get<1>(t1) == 'c');

    const int increase = 2; bool status = true;

    auto addWithTest =  [increase, &status] ( auto& t) {
        t += increase;
        if(int(t) > int('e')) {
            status = false;
        }
    };

    for_each(t1, addWithTest);
    assert(std::get<0>(t1) == 14);
    assert(status);
    for_each(t1, addWithTest);
    assert(std::get<0>(t1) == 16);
    assert(!status);

    std::tuple<int*, char*> t3{ };

    for_each(t3, increment3);
}
