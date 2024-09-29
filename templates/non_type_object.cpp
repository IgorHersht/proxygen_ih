
#include <tuple>
#include <array>
#include <type_traits>

struct C1 {
    consteval bool isValid() {
        return _id1 && _id2;
    }
    int _id1{0};
    int _id2{0};
};

struct C2 {
    consteval bool isValid() {
        return _id1.isValid() && _id2;
    }
    C1 _id1{0};
    int _id2{0};
};

constexpr std::array ar1 {1, 2};
constexpr std::array ar2 {1, 3};

constexpr std::pair p1 {1, 'x'};
constexpr std::pair p2 {1, 'y'};

constexpr std::tuple t1 {1, 'x'};
constexpr std::tuple t2 {1, 'y'};

template <C1 c1> struct C1T {};
template <C2 c2> struct C2T {};
template <auto ID> struct AUTO {};


int main() {

    static_assert( std::is_same_v<C1T<C1{1,2}>, C1T<C1{1,2}> >);
    static_assert( !std::is_same_v<C1T<C1{2,1}>, C1T<C1{1,2}> >);

    static_assert( std::is_same_v<C2T< C2{C1{1,2}, 3}>, C2T< C2{C1{1,2}, 3}> >);
    static_assert( !std::is_same_v<C2T< C2{C1{4,2}, 3}>, C2T< C2{C1{1,2}, 3}> >);

    static_assert( std::is_same_v<AUTO<ar1>, AUTO<ar1> >);
    static_assert( !std::is_same_v<AUTO<ar1>, AUTO<ar2> >);

    static_assert( std::is_same_v<AUTO<p1>, AUTO<p1> >);
    static_assert( !std::is_same_v<AUTO<p1>, AUTO<p2> >);
/* error: ‘std::tuple<int, char>’ is not a valid type for a template non-type parameter because it is not structural
    static_assert( std::is_same_v<AUTO<t1>, AUTO<t1> >);
    static_assert( !std::is_same_v<AUTO<p1>, AUTO<p2> >);
    */
}
