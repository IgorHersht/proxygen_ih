// https://www.scs.stanford.edu/~dm/blog/param-pack.html

template<typename ...Lambdas> struct multilambda : Lambdas... {
  using Lambdas::operator()...;
};
template<typename ...Lambdas> multilambda(Lambdas...) -> multilambda<Lambdas...>;

#include <tuple>

template<typename T, template<typename...> typename Tmpl>
concept is_template = decltype(multilambda{
    []<typename ...Ts>(const Tmpl<Ts...> &) { return std::true_type{}; },
    [](const auto &) { return std::false_type{}; },
  }(std::declval<T>()))::value;

#include <variant>
#include <vector>
static_assert(is_template<std::tuple<int, long>, std::tuple>);
static_assert(is_template<const std::tuple<int, long> &, std::tuple>);
static_assert(is_template<std::vector< char>, std::vector>);
// does not work for sts::array
static_assert(!is_template<std::tuple<int, long>, std::variant>);

/// test
#include <string>
#include <iostream>
int  main() {
  using namespace std::string_literals;
  std::tuple t (1, true, "hello"s, 3.0);
  constexpr multilambda action {
    [](int i) { std::cout << i << std::endl; },
    [](double d) { std::cout << d << std::endl; },
    [](bool b) { std::cout << (b ? "yes" : "no") <<std::endl; },
    [](std::string s) { std::cout << s.size() << " bytes\n"; },
  };
  apply([action](auto ...v) {
    (action(v), ...);  // unary right fold
  }, t);
  // 1
  // yes
  // 5 bytes
  // 3
}
