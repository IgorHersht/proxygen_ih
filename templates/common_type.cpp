#include "vector.hpp"
#include <iostream>
#include <array>
#include <string_view>
#include <type_traits>

// https://levelup.gitconnected.com/c-templates-type-identity-technique-f2b427759403
/*
std::common_type is a templates utility that gives a common type of given types 
 that all of them can be implicitly converted to
 */
template<class T>
T add_impl(T a, T b) {
    return a + b;
}
template<class T1, class T2>
typename std::common_type_t<T1, T2>
add(T1 t1, T2 t2) {
    return add_impl<std::common_type_t<T1,T2>>(t1, t2);
}
int main() {
    // foo(4.2, 1); // error, deduced conflicting types for 'T'
    std::cout << add(4.2, 1) << '\n';  // OK, calls bar<double>
}
