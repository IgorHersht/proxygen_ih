#include <tuple>
#include <type_traits>

template <typename T, typename... Us>
struct is_tuple_has_type : std::false_type {};;

template <typename T, typename... Us>
struct is_tuple_has_type<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...> {};

template <typename T, typename... Us> constexpr static bool is_tuple_has_type_v = is_tuple_has_type< T,Us... >::value;
template <typename T, typename... Us> concept tuple_has_type = is_tuple_has_type_v<T,Us...>;


int main() {
    static_assert(is_tuple_has_type_v<int,std::tuple<int, char>>);
    static_assert(is_tuple_has_type_v<char,std::tuple<int, char>>);
    static_assert(!is_tuple_has_type_v<bool,std::tuple<int, char>>);
    static_assert(!is_tuple_has_type_v<long,std::tuple<int, char>>);

}
