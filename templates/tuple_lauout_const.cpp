#include <cstddef>
#include <tuple>

template <std::size_t I, typename Tuple>
 consteval std::size_t element_offset() {
    union u {
        char a[sizeof(Tuple)]{};
        Tuple t;
    } x;
    auto* p = std::addressof(std::get<I>(x.t));
    for (std::size_t i = 0; sizeof(Tuple) ; ++i) {
        if (static_cast<void*>(x.a + i) == p) {
            return i;
        }
    }
}
template <typename ET, typename Tuple>
 consteval std::size_t element_offset() {
    union u {
        char a[sizeof(Tuple)]{};
        Tuple t;
    } x;
    auto* p = std::addressof(std::get<ET>(x.t));
    for (std::size_t i = 0; i < sizeof(Tuple) ; ++i) {
        if (static_cast<void*>(x.a + i) == p) {
            return i;
        }
    }
}
//test

int main() {

    static_assert(16 == element_offset<1, std::tuple<char, int, long, short>>());
    static_assert(16 == element_offset<int, std::tuple<char, int, long, short>>());
}

