
#include <tuple>
#include <cstddef>

// elements default constructable
template <std::size_t I, typename Tuple>
 constexpr  std::size_t element_offset() {
     if (std::is_constant_evaluated()) {
        union u {
            constexpr u(){}
            char a[sizeof(Tuple)]{};
            Tuple t;
        } x;
        auto* p = std::addressof(std::get< I >(x.t));
        for (std::size_t i = 0; sizeof(Tuple) ; ++i) {
            if (static_cast<void*>(x.a + i) == p) {
                return i;
            }
        }
        return sizeof(Tuple);
    }else {
        Tuple t;
        return (size_t)(&std::get<I>(t)) - (size_t)(&(t));
    }
}

/// test
 #include <assert.h>
#include <boost/pfr.hpp>

struct In {
    std::array<short, 3> ar{};
};

int main() {
    struct C {char ch; std::array<double, 3> ar; char ch2 ; In in; int i;};
    using Tuple = decltype(boost::pfr::structure_to_tuple(std::declval<C>()));

    static_assert(sizeof(C) == sizeof(Tuple));
    static_assert(alignof(C) == alignof(Tuple));

    Tuple tp;
    auto of0 = element_offset<0, Tuple> ();  constexpr auto of0_c = element_offset<0, Tuple> ();
     assert(of0 == of0_c);
    auto of1 = element_offset<1, Tuple> ();  constexpr auto of1_c = element_offset<1, Tuple> ();
    assert(of1 == of1_c);
    auto of2 = element_offset<2, Tuple> ();  constexpr auto of2_c = element_offset<2, Tuple> ();
    assert(of2 == of2_c);
    auto of3 = element_offset<3, Tuple> ();  constexpr auto of3_c = element_offset<3, Tuple> ();
    assert(of3 == of3_c);
    auto of4 = element_offset<4, Tuple> ();  constexpr auto of4_c = element_offset<4, Tuple> ();
    assert(of4 == of4_c);
}
