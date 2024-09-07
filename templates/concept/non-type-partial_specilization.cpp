
#include <tuple>
#include <array>
#include <memory>
template <class Tuple, class F> constexpr decltype(auto) for_each1(Tuple&& tuple, F&& f) {
    return [] <std::size_t... I> (Tuple&& tuple, F&& f, std::index_sequence<I...>) {
        (f(std::get<I>(tuple)), ...);
        return f;
    }(std::forward<Tuple>(tuple), std::forward<F>(f), std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

template <typename TupleT,  typename Fn>
void for_each2(TupleT&& tp, Fn&& fn)  {
    std::apply (
        [&fn]<typename ...T>(T&& ...args) requires (... && std::invocable<Fn, T>) {
        (std::forward<Fn>(fn)(std::forward<T>(args)), ...);
        }, std::forward<TupleT>(tp)
    );
}


////test

std::tuple t{ 10, 5.2, 3l, (short) 2};
auto increment = []( auto& t) { ++t ; };



int main() {

 auto s1 = for_each1(t,increment );
 int i =1;


}

