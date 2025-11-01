#include <tuple>
#include <array>
#include <memory>
template <class Tuple, class F>
constexpr void for_each1(Tuple&& tuple, F&& f) {
    return [] <std::size_t... I> (Tuple&& tuple, F&& f, std::index_sequence<I...>) {
        (f(std::get<I>(tuple)), ...);
    }(std::forward<Tuple>(tuple), std::forward<F>(f), std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

template <typename TupleT,  typename Fn>
constexpr void for_each2(TupleT&& tp, Fn&& fn)  {
    std::apply (
        [&fn]<typename ...T>(T&& ...args) requires (... && std::invocable<Fn, T>) {
            (std::forward<Fn>(fn)(std::forward<T>(args)), ...);
        }, std::forward<TupleT>(tp)
    );
}

// test
std::tuple t{ 10, 5.2, 3l, (short) 2};
auto increment = []( auto& t) { ++t ; };
auto check = []( auto& t) {static_assert(sizeof(t) > 2); };

struct C {
    consteval C() {
        for_each1(t,check );
        for_each2(t,check );
    }
};
constexpr C c{};

int main() {
    for_each1(t,increment );
    for_each2(t,increment );
    for_each2(t,check );
}
