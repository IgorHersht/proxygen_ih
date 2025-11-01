//https://www.fluentcpp.com/2019/03/08/stl-algorithms-on-tuples/
#include <tuple>
#include <utility>
template<typename Tuple, typename F> constexpr std::size_t  tuple_find(const Tuple &tuple, F &&f){
  return std::apply([&f](const auto &...ETs) {
    std::size_t size = 0;
    ((std::forward<F>(f)(ETs) || (++size, false)) || ...);
    return size;
  }, tuple);
}

template<typename Tuple, typename F> constexpr std::size_t  tuple_find2(const Tuple &tuple, F &&f){
  return std::apply([&f](const auto &...ETs) {
    std::size_t index = 0;
    ((std::forward<F>(f)(ETs) || (++index == std::tuple_size_v<Tuple>)) || ...);
    return index;
  }, tuple);
}

// test
using Tuple = std::tuple<int, long,char>;

int main() {
  constexpr std::size_t pos = tuple_find(Tuple{},
   [](auto element) {return std::cmp_greater(sizeof(element), 4); } );

  constexpr std::size_t pos2 = tuple_find2(Tuple{},
  [](auto element) {return std::cmp_greater(sizeof(element), 4); } );
  int i =1;

}
