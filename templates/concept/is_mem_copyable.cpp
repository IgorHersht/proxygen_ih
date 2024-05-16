#include <tuple>
#include <utility>
#include <array>
#include <type_traits>
#include <string.h>
#include <string>
using MT = std::tuple<  double, char, std::array<int,2>  >;
std::array<std::byte, sizeof(MT)> ar{};
void f() {
    MT mt{3.2, 'x', {5,6} };
    memcpy((void*)&ar, &mt, sizeof(MT));
    int i =1;
}
//template<typename T, typename U = void> struct is_mem_copyable : std::false_type {};
//template<typename T> struct is_mem_copyable<T, std::void_t<typename T::mem_copyable>>:std::true_type {};

template <typename T, typename = void> constexpr bool is_mem_copyable{std::is_trivially_copyable_v<T>};
template <typename T> constexpr bool is_mem_copyable< T,std::void_t<typename T::mem_copyable>> = true;


struct C {
    enum mem_copyable{};
    //using  mem_copyable = bool;
};
struct D{};

int main()
{
    constexpr bool is1 = is_mem_copyable<C>;
    constexpr bool is2 = is_mem_copyable<std::string>;
    constexpr bool is3 = is_mem_copyable<D>;
    constexpr bool is4 = is_mem_copyable<int>;
    MT out{};
    f();
    memcpy((void*)&out, (const void *) &ar, sizeof(MT));
    int i =1;
}
