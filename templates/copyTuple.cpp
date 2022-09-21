#include <tuple>
#include <utility>
#include <array>

std::tuple< char, int, bool, float, double > from { 'x', 1, true, 4.4f, 7.8   };
std::tuple<  double, char, std::array<char,5> > to { 0.0, '\0', {}  };

template <typename T, typename Tuple>
struct has_type;

template <typename T, typename... Us>
struct has_type<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...> {};

template<typename T, typename Tuple  > void copyIf( T value, Tuple& to  ) {
    if constexpr (has_type< T,Tuple>::value) {
        std::get<T>(to) = value;
    } else  {
    }
}

template<typename From, typename To> void copyTuple( const From& from, To& to ) {
    std::apply([&to](auto&&... args) {((
            copyIf(args, to)
    ), ...);}, from);
}


int main()
{
    std::tuple<  double, char, std::array<char,5>  > out;
    copyTuple(from, out);
    int i =1;
}
