//https://www.fluentcpp.com/2019/03/08/stl-algorithms-on-tuples/
//https://www.cppstories.com/2022/tuple-iteration-apply/
//https://www.fluentcpp.com/2019/03/08/stl-algorithms-on-tuples/
#include <tuple>
template <typename TupleT,  typename Fn>
consteval  decltype(auto) for_each_const(TupleT tp, Fn fn)  {
    return std::apply (
          [fn]<typename ...T>(T ...args) requires (... && std::invocable<Fn, T>) {
             return (fn(args) + ...);
         }, tp
     );
}

constexpr std::tuple tup{ 10, short(2)};
auto increment = []( auto t) {
    return t + 1;
};

constexpr auto r = for_each_const(tup,increment );
static_assert(r == 14);

int main() {

}
