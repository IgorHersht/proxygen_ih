
#include <utility>
#include <tuple>
#include <vector>
#include <array>


namespace _detail
{
    template<typename, template<typename...> typename> struct is_instance_of: public std::false_type{};
    template< template<typename...> typename U, typename ... Ts> struct is_instance_of<U<Ts...>, U> : public std::true_type{};
    // std::array need special because has non-type param
    template <typename T>       struct is_array : std::false_type{};
    template <typename T, std::size_t N> struct is_array<std::array<T,N>> : std::true_type{};
}

template<typename T, template<typename...> typename U> using is_instance_of = _detail::is_instance_of<std::decay_t<T>, U>;
template<typename T, template<typename...> typename U> static constexpr bool is_instance_of_v = is_instance_of<T, U>::value;
template<typename T, template<typename...> typename U> concept instance_of = is_instance_of_v<T, U>;

template<typename T> static constexpr bool is_tuple_v = is_instance_of_v<T, std::tuple>;
template<typename T> concept same_as_tuple = is_tuple_v<T>;

template<typename T> static constexpr bool is_vector_v = is_instance_of_v<T, std::vector>;
template<typename T> concept same_as_vector = is_vector_v<T>;

template <typename T> static constexpr bool is_array_v = _detail::is_array<std::decay_t<T>>::value;
template<typename T> concept same_as_array = is_array_v<T>;

// test
using Tuple1 = std::tuple<char, int>;
using Vector1 = std::vector<char>;
using Array1 = std::array<int, 5>;

static_assert( is_instance_of_v<Tuple1, std::tuple>);
static_assert( instance_of<Tuple1, std::tuple>);
static_assert( is_tuple_v<Tuple1>);
static_assert( same_as_tuple<Tuple1>);
static_assert( is_vector_v<Vector1>);
static_assert( same_as_vector <Vector1>);
static_assert( is_array_v<Array1>);
static_assert( same_as_array <Array1>);

class C{};
static_assert( !is_instance_of_v<Tuple1, std::vector>);
static_assert( !instance_of<Tuple1, std::vector>);
static_assert( !instance_of<C, std::tuple>);
static_assert( !instance_of<int, std::tuple>);
static_assert( !is_tuple_v<C>);
static_assert( !same_as_tuple<int>);

int  main() {

}
