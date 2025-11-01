#include <iostream>
#include <cstdint>
#include <memory>

// version of the gist at: https://gist.github.com/graphitemaster/494f21190bb2c63c5516
// original version by graphitemaster

template <typename T1, typename T2>
struct offset_of_impl {
    union U {
        char c;
        T1 m; // instance of type of member
        T2 object;
        constexpr U() : c(0) {} // make c the active member
    };
    static constexpr U u = {};

    static constexpr std::ptrdiff_t offset(T1 T2::*member) {
        // The following avoids use of reinterpret_cast, so is constexpr.
        // The subtraction gives the correct offset because the union layout rules guarantee that all
        // union members have the same starting address.
        // On the other hand, it will break if object.*member is not aligned.
        // Possible problem: it uses std::addressof on non-active union members.
        // Please let us know at the gist if this is defined or undefined behavior.
        // [EDIT: it is undefined, but for the following reason: expr.add-5.sentence-2
        // "If the expressions P and Q point to, respectively, elements x[i] and x[j] of
        // the same array object x, the expression P - Q has the value i - j; otherwise, the behavior is undefined."]
        return (std::addressof(offset_of_impl<T1, T2>::u.object.*member) -
               std::addressof(offset_of_impl<T1, T2>::u.m)) * sizeof(T1);
    }
};

template <typename T1, typename T2>
constexpr typename offset_of_impl<T1, T2>::U offset_of_impl<T1, T2>::u;

template <typename T1, typename T2>
inline constexpr std::ptrdiff_t offset_of(T1 T2::*member) {
    return offset_of_impl<T1, T2>::offset(member);
}

struct S {
    S(int a_, int b_, int c_) : a(a_), b(b_), c(c_) {}
    S() = delete;
    int a;
    int b;
    int c;
};

int main()
{
    constexpr auto x = offset_of(&S::b);
    std::cout << x;
}
