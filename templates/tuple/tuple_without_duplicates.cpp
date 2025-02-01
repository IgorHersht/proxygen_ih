#include <string>
#include <type_traits>
#include <tuple>

namespace _impl {
  template <class Haystack, class Needle> struct contains;

  template <class Car, class... Cdr, class Needle> struct contains<std::tuple<Car, Cdr...>, Needle> : contains<std::tuple<Cdr...>, Needle> {};

  template <class... Cdr, class Needle> struct contains<std::tuple<Needle, Cdr...>, Needle> : std::true_type {};

  template <class Needle> struct contains<std::tuple<>, Needle> : std::false_type {};

  template <class Out, class In> struct filter;

  template <class... Out, class InCar, class... InCdr> struct filter<std::tuple<Out...>, std::tuple<InCar, InCdr...>> {
    using type = typename std::conditional<
      contains<std::tuple<Out...>, InCar>::value
      , typename filter<std::tuple<Out...>, std::tuple<InCdr...>>::type
      , typename filter<std::tuple<Out..., InCar>, std::tuple<InCdr...>>::type
    >::type;
  };

  template <class Out> struct filter<Out, std::tuple<>> {
    using type = Out;
  };
}

template <class Tuple> using tuple_without_duplicates = typename  _impl::filter<std::tuple<>, Tuple>::type;
template <typename Tuple> constexpr bool IsUnique = std::is_same_v<Tuple, tuple_without_duplicates<Tuple>>;

int main()
{
    using Tuple = std::tuple<int, double, int, double, std::string, std::string>;
    using FilteredTuple = tuple_without_duplicates<Tuple>;
    static_assert(std::is_same_v<FilteredTuple, std::tuple<int, double, std::string>>);

  static_assert(IsUnique<FilteredTuple>);
  static_assert(!IsUnique<Tuple>);
}
