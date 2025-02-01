#include <iostream>
#include <array>
#include <string>

template <typename T>       struct is_non_type:std::false_type{};
template <template <auto ...>typename T, auto... N> struct is_non_type<T<N...>> :std::true_type{};

template<int i> struct C1{};
template<int i, char ch> struct C2{};

template <typename T>       struct is_type:std::false_type{};
template <template <typename ...>typename TT, typename... T> struct is_type<TT<T...>> :std::true_type{};

template<typename T> struct T1{};



int main() {

    bool b1 = is_non_type<C1<5>>::value;
    bool b2 = is_non_type<C2<5,'c'>>::value;
    bool b3 = is_type<T1<int>>::value;

    return 0;
}
