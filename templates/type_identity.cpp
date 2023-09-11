#include "vector.hpp"
#include <iostream>
#include <array>
#include <string_view>
#include <type_traits>

// https://levelup.gitconnected.com/c-templates-type-identity-technique-f2b427759403
/*
 * template <class T>
struct type_identity {
    using type = T;
};
 f template parameter T appears on the left side of :: scope resolution operator in
 an expression of a template function parameter, compiler does not try to deduce the type by deduction process.
 Instead, it uses the result of deduction process from somewhere else
 */
template <class T>
T foo(T a, T b) {
    return a + b;
}

template <class T>
T bar(T a, std::type_identity_t<T> b) {
    return a + b;
}

int main() {
    // foo(4.2, 1); // error, deduced conflicting types for 'T'
    std::cout << bar(4.2, 1) << '\n';  // OK, calls bar<double>
}
