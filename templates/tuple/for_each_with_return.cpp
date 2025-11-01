//https://www.cppstories.com/2022/tuple-iteration-apply/
//https://www.fluentcpp.com/2019/03/08/stl-algorithms-on-tuples/
#include <tuple>
template <typename TupleT, typename Fn>
constexpr  auto transform_tuple(TupleT&& tp, Fn&& fn) {
    return std::apply
    (
        [&fn]<typename ...T>(T&& ...args) {
            return std::make_tuple(fn(std::forward<T>(args))...);
        },
        std::forward<TupleT>(tp)
    );
}

int main() {

    // not constexpr
    std::tuple tup{ 10, short(2)};
    auto increment = []( auto& t) {
        return ++t; // changing t => not constexpr
    };
    auto r = transform_tuple(tup,increment );

    // constexpr
    constexpr std::tuple tup2{ 10, short(2)};
    auto increment2 = []( auto& t) {
        return t +1;//constexpr
    };
    constexpr auto r2 = transform_tuple(tup2,increment2 );
    int i =1;
}
