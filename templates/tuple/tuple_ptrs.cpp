##include <tuple>


template<typename Tuple> using tuple_ptrs =
  decltype(std::apply(
      [](auto ...Ts) { return std::tuple(&Ts...); }, 
    std::declval<Tuple>()) );

//test
static_assert(std::is_same_v<tuple_ptrs<std::tuple<int, char>>, std::tuple<int*, char*>>);

int main() {

}

