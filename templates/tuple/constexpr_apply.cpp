template <class P>
constexpr auto dot_sum(P p) {
    return std::apply([&](auto... p_elems){
      return (... +  p_elems);
    }, p);
}

//test
int main()
{
    constexpr  std::tuple p{2, 3.0};
  constexpr  auto r = dot_sum(p);
  static_assert(r == 5.0);
    return 0;
}
