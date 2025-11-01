#include <tuple>
template <typename TupleT,  typename Fn>
consteval  decltype(auto) tuple_to_tuple_const1(TupleT tp, Fn fn)  {
    TupleT newTp{};
    std::apply([fn,&newTp](auto... p_elems){
         std::apply([fn, p_elems..., &newTp](auto&... q_elems){
             (... ,(q_elems = fn(p_elems)));
        }, newTp);
    }, tp);
    return newTp;
}
//test
using Tp = std::tuple<int,short>;
constexpr Tp tup{ 10, short(2)};
auto increment = []( auto t) { return t + 1; };
constexpr Tp newTup= tuple_to_tuple_const1(tup,increment );
static_assert(newTup == Tp{11,3} );

int main() {

}

