
#include <tuple>
#include <array>
#include <cstring>
#include <string>
#include <memory>
#include <assert.h>

template <typename... TS>  struct is_tuple :public std::false_type {};
template <typename... TS> struct is_tuple<std::tuple<TS...>> :std::true_type {};// specilization on tuple
template <typename... TS> constexpr static bool is_tuple_v = is_tuple<TS...>::value;
template <typename... TS> concept same_as_tuple = is_tuple_v<TS...>;

template<size_t Idx,same_as_tuple T>
 size_t tuple_element_offset() {
    return(size_t)( &std::get<Idx>(*reinterpret_cast<T*>(0)) );
}

template<typename ET,same_as_tuple T>
 size_t tuple_element_offset() {
    return (size_t)( &std::get<ET>(*reinterpret_cast<T*>(0)) );
}
//// test
struct C {
    char ch = 'x';
    std::array<int,5> ar{3, 7, 25, 4, 88 };
    double d{0.45};
};
// reverse order!!!
using MyTuple = std::tuple< double, std::array<int,5>, char >;

int main() {
    static_assert(sizeof(MyTuple)== sizeof(C));
    MyTuple tuple;
    C c1;
    memcpy(&tuple,  &c1, sizeof(C));

    assert(offsetof(C,d) == (tuple_element_offset<0, MyTuple>()) );
    assert(offsetof(C,ar) == (tuple_element_offset<1, MyTuple>()) );
    assert(offsetof(C,ch) == (tuple_element_offset<2, MyTuple>()) );
}



