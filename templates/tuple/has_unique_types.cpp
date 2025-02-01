#include <string>
#include <type_traits>
#include <tuple>
#include <boost/pfr.hpp>

template <typename... TS>  struct is_tuple :public std::false_type {};
template <typename... TS> struct is_tuple<std::tuple<TS...>> :std::true_type {};// specilization on tuple
template <typename... TS> constexpr static bool is_tuple_v = is_tuple<TS...>::value;
template <typename... TS> concept same_as_tuple = is_tuple_v<TS...>;
template <typename T> concept same_as_class = std::is_class_v<T> && (! is_tuple_v<T>) ;
template<class> inline constexpr bool always_false_v = false;

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
//
template <same_as_tuple Tuple> using without_duplicates = typename  _impl::filter<std::tuple<>, Tuple>::type;
template <same_as_tuple Tuple> constexpr bool is_unique_tuple_types = std::is_same_v<Tuple, without_duplicates<Tuple>>;
//

template <typename T> consteval bool hasUniqueTypes() {
  if constexpr(same_as_tuple<T>) {
    return is_unique_tuple_types<T>;
  } else if constexpr(same_as_class<T>) {
    using Tuple = decltype(boost::pfr::structure_to_tuple(std::declval<T>()));
    return is_unique_tuple_types<Tuple>;
  } else {
    static_assert(always_false_v<T>, "unknown type");
  }
}

template <typename T> constexpr bool has_unique_types_v = hasUniqueTypes<T>();


int main()
{
    using Tuple = std::tuple<int, double, int, double, std::string, std::string>;
    using FilteredTuple = without_duplicates<Tuple>;
    static_assert(std::is_same_v<FilteredTuple, std::tuple<int, double, std::string>>);

  static_assert(is_unique_tuple_types<FilteredTuple>);
  static_assert(!is_unique_tuple_types<Tuple>);
////
  struct C1 { int i; double d; int i2; double d2; std::string s1; std::string s2;};
  struct C2 { int i; double d; std::string s2;};

  static_assert(!has_unique_types_v<Tuple>);
  static_assert(has_unique_types_v<FilteredTuple>);

  static_assert(!has_unique_types_v<C1>);
  static_assert(has_unique_types_v<C2>);

}
