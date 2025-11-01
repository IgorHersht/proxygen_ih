#include <tuple>

template <class P, class Q>
auto dot_product(P p, Q q) {
    return std::apply([&](auto... p_elems){
        return std::apply([&](auto... q_elems){
            return (... + (p_elems * q_elems));
        }, q);
    }, p);
}

//test
std::tuple p{2, 3.0};
std::tuple q{3.0, 4};
int main()
{
  constexpr  auto r = dot_product(p, q);
  static_assert(r == 18);
    return 0;
}
