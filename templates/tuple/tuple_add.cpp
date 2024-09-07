#include <string>
#include <iostream>
#include <set>

auto tuple_mult(auto scalar, auto tpl){
    return apply([&scalar]<typename ...T>(T...t) {
        return std::tuple(T(scalar * t)...);
      }, tpl);
}

template<typename T> auto  tuple_add(const T &a, const T&b){
    return [&a, &b]<std::size_t ...I>(std::index_sequence<I...>) {
        return std::tuple(get<I>(a) + get<I>(b)...);
    }(std::make_index_sequence<std::tuple_size_v<T>>{});
}

int  main() {
    auto t = std::tuple(1, 2U, 4.0);
    t = tuple_mult(2, t);
    std::cout << get<0>(t) << " "
              << get<1>(t) << " "
              << get<2>(t) << std::endl;
    // prints:
    // 2 4 8
    auto t2 = std::tuple(1, 2U, 4.0);
    t2 = tuple_add(t, tuple_mult(10, t));
    std::cout << get<0>(t2) << " "
              << get<1>(t2) << " "
              << get<2>(t2) << std::endl;
    // prints:
    // 22 44 88
}
