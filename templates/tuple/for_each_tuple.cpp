
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

/// test
#include <string>
#include <concepts>
#include <cassert>

auto increment1 = []( auto& t) { ++t ; };

struct increment2 {
    void operator()(auto& t) { ++t;}
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
    std::tuple<int, std::string> t2{ 10,std::string("10")};
    auto addWithTest = [increase, &status] < typename T>  ( T& t) {
        if constexpr (std::same_as<T, int>) {
           t +=  increase;
            status = t < 13;
        } else {
            int ti = std::stoi(t);
            ti +=  increase;
            status = ti < 13;
            t = std::to_string(ti);
        }
    };
     for_each(t2, addWithTest);
     assert(std::get<0>(t2) == 12);
    assert(std::get<1>(t2) == std::string("12"));
    assert(status);
    for_each(t2, addWithTest);
    assert(std::get<0>(t2) == 14);
    assert(std::get<1>(t2) == std::string("14"));
    assert(!status);
}



