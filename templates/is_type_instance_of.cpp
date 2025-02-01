#include <vector>
#include <string>

template <typename >       struct is_type_of:std::false_type{};
template <template <auto ...>typename T, auto... Ns> struct is_type_of<T<Ns...>> :std::true_type{};
template <template <typename ...>typename TT, typename... Ts> struct is_type_of<TT<Ts...>> :std::true_type{};

////////////////////////////////////////////

template <typename, template <typename ...>typename > struct is_type_instance_of : std::false_type{};
template <template <typename ...>typename TT,  typename...Ts> struct is_type_instance_of<TT<Ts...>, TT> :std::true_type{};

template <typename, template <auto ...>typename  > struct is_non_type_instance_of : std::false_type{};
template <template <auto ...>typename TT,  auto...Ns> struct is_non_type_instance_of<TT<Ns...>, TT> :std::true_type{};

//test
template<typename T> struct T1{};
template<int i> struct C1{};
template<int i, char ch> struct C2{};

int main() {

    bool b1 = is_type_of<C1<5>>::value;
    bool b2 = is_type_of<C2<5,'c'>>::value;
    bool b3 = is_type_of<T1<int>>::value;

    bool b4 = is_type_instance_of<std::vector<int>, std::vector>::value;
    bool b5 = is_non_type_instance_of<C1<5>,C1>::value;
    bool b6 = is_non_type_instance_of<C2<5, 'x'>,C2>::value;

    bool b7 = is_type_instance_of<std::vector<int>, T1>::value;
    bool b8 = is_non_type_instance_of<C2<5, 'x'>,C1>::value;

    return 0;
}
