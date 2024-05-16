#include <iostream>
#include <type_traits>
#include <string>
#include <tuple>
#include <variant>
#include <assert.h>

//The compound-requirement in C2 requires that *x is a valid expression, that typename T::inner is a valid
//type, and that std::same_as<decltype((*x)), typename T::inner> is satisfied.
template<typename T> concept C2 = requires(T x) {
    {*x} -> std::same_as<typename T::inner>;
};
struct Com {
    using inner = Com&;
};



// The following concept ensures a specialization of A is deduced.
template <class T> struct A {
    explicit A(const T&, ...) noexcept; // #1
    A(T&&, ...); // #2
};
template <class> class AA;
template <class V> class AA<A<V>> { };
template <class T> concept deduces_A = requires { sizeof(AA<T>); };

//test
// compound-requirement


int main() {
}
