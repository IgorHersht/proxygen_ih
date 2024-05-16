#include <iostream>
#include <type_traits>
#include <string>
#include <tuple>
#include <variant>

template < typename T> concept has_static_bool_true = requires {
    T::IsMemCopyable;
    requires (T::IsMemCopyable == true);
}  ;
//
template < typename T> concept mem_copyable = std::is_trivially_copyable_v<T> || (requires { T::IsMemCopyable; }  && (T::IsMemCopyable == true) ) ; // consept
// the same using nested requrement
template < typename T> concept mem_copyable2 = std::is_trivially_copyable_v<T> || (requires
{
    T::IsMemCopyable;
    requires (T::IsMemCopyable == true);
} ) ;
// the same
template < typename T> concept mem_copyable3 = std::is_trivially_copyable_v<T> || has_static_bool_true<T>;

template < typename T> struct is_mem_copyable : public std::integral_constant<bool, mem_copyable<T>> {}; // type_trait
template < typename T> constexpr static bool is_mem_copyable_v = is_mem_copyable<T>::value; //constexpr static bool

template < typename T> consteval bool get_is_mem_copyable_v() {
    if constexpr (mem_copyable<T>) {
        return true;
    } else {
        return false;
    }
}
template < typename T> constexpr static bool is_mem_copyable2_v = get_is_mem_copyable_v<T>(); // the same as is_mem_copyable_v

template < typename... TS> concept all_mem_copyable = std::conjunction< is_mem_copyable<TS>...>::value;
template < typename... TS> concept all_mem_copyable2 = (... && is_mem_copyable_v<TS>); // the same using using fold expresion
template < typename ... TS> struct is_all_mem_copyable : public std::integral_constant<bool, all_mem_copyable<TS...>> {};
template < typename ... TS> constexpr static bool is_all_mem_copyable_v = is_all_mem_copyable<TS...>::value;

template <typename... TS>  struct is_tuple :public std::false_type {};
template <typename... TS> struct is_tuple<std::tuple<TS...>> :std::true_type {};// specilization on tuple
template <typename... TS> constexpr static bool is_tuple_v = is_tuple<TS...>::value;
template <typename... TS> concept same_as_tuple = is_tuple_v<TS...>;

template <typename... TS>  struct is_tuple_mem_copyable : public std::false_type {};
template< typename... TS>  struct is_tuple_mem_copyable<std::tuple<TS...>> : public std::integral_constant<bool, all_mem_copyable<TS...>> {}; // specilization on tuple
template< typename... TS> constexpr static bool is_tuple_mem_copyable_v = is_tuple_mem_copyable<TS...>::value;
template< typename... TS> concept tuple_mem_copyable = is_tuple_mem_copyable_v<TS...>;

// test
struct C0 {
    int i;
};
struct C1 {
    std::tuple<int, char> t;
};
struct C2 {
    std::tuple<int, char> t;
    constexpr static bool IsMemCopyable = is_tuple_mem_copyable_v<decltype(t)>;
};

struct C3 {
    std::tuple<int, std::string> t;
    constexpr static bool IsMemCopyable = is_tuple_mem_copyable_v<std::tuple<int, std::string>>;
};

int main() {
    static_assert(!is_mem_copyable_v<std::string>);
    static_assert(is_mem_copyable_v<int>);
    static_assert(is_mem_copyable_v<C0>);
    static_assert(!is_mem_copyable_v<C1>);
    static_assert(is_mem_copyable_v<C2>);
    static_assert(!is_mem_copyable_v<C3>);

    static_assert(is_all_mem_copyable_v<char, int>);
    static_assert(!is_all_mem_copyable_v<char, std::string>);

    using T1 = std::tuple<int, char>;
    using T2 = std::tuple<int, std::string>;
    static_assert(is_tuple_v<T1>);
    static_assert(!is_tuple_v<int>);
    static_assert(!is_tuple_v<C1>);

    static_assert( is_tuple_mem_copyable_v<T1>);
    static_assert( !is_tuple_mem_copyable_v<T2>);
    static_assert( !is_tuple_mem_copyable_v<C1>);
    static_assert( !is_tuple_mem_copyable_v<int>);
}
// 
