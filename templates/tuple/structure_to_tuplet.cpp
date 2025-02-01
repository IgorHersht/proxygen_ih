#include <boost/pfr.hpp>

int main() {
    struct C {char ch{'x'}; int i{1}; std::array<double, 2> d {2.0, 3.0}; float f {4.0f};};

    constexpr C c;
    // boost::pfr::structure_to_tuple
    constexpr auto t1 = (boost::pfr::structure_to_tuple(c));;
    static_assert(std::get<0>(t1) == 'x');
    static_assert(std::get<1>(t1) == 1);
    static_assert(std::get<2>(t1) == std::array<double, 2>{2.0, 3.0});
    static_assert(std::get<3>(t1) == 4.0);

    constexpr auto t2 = std::make_tuple(c.ch,c.i,c.d,c.f);
    static_assert(std::get<0>(t2) == 'x');
    static_assert(std::get<1>(t2) == 1);
    static_assert(std::get<2>(t2) == std::array<double, 2>{2.0, 3.0});
    static_assert(std::get<3>(t2) == 4.0);
}


