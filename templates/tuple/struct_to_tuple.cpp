#include <tuple>
#include <type_traits>
#include <cstring>
#include <string>
#include <memory>
#include <assert.h>
#include <array>

template <class... Ts> static consteval size_t get_max_size()   { return std::max({sizeof(Ts)...}); }
template <class... Ts> static consteval size_t get_max_size(std::tuple<Ts...>*  )  { return get_max_size<Ts...>(); }
template <class... Ts> static consteval size_t get_max_alignment() { return std::max({alignof(Ts)...}); }
template <class... Ts> static consteval size_t get_max_alignment(std::tuple<Ts...>*  )  { return get_max_alignment<Ts...>(); }

template <class... Ts> static consteval std::array<size_t,sizeof...(Ts)> getAlignments( ) { return {alignof(Ts)...}; }
template <class... Ts> static consteval std::array<size_t,sizeof...(Ts)> getAlignments( std::tuple<Ts...>* ) { return getAlignments<Ts...>(); }

template <class... Ts> static consteval bool ifSortedByAlignment( ) {
    constexpr size_t size = sizeof...(Ts);
    if constexpr(size == 1) {
        return true;
    } else {
        constexpr std::array<size_t,size> ar {alignof(Ts)...};
        for(size_t pos = 1; pos <size; ++pos  ) {
            if(ar[pos] < ar[pos -1] ) {
                return false;
            }
        }
        return true;
    }
}
template <class... Ts> static consteval bool ifSortedByAlignment(std::tuple<Ts...>*  ) { return ifSortedByAlignment<Ts...>(); }

template<typename StructT> struct fact_to_tuple{
    using Fields = typename StructT::Fields;
    static_assert(ifSortedByAlignment((Fields*)nullptr));
    constexpr static inline std::array fieldNames = StructT::fieldNames;
    static_assert(sizeof(StructT) == sizeof(Fields));
    static_assert( std::tuple_size_v<Fields> == std::tuple_size_v<decltype(fieldNames)>);
    // check if sorted
};

//test
// make strong_string
#define FIELD_NAME(Field) (#Field)

struct C {
    float f{3.0};
    int i{1};
    char ch{'2'};
    using Fields = decltype(std::make_tuple( ch, i, f )); // reverse order
    constexpr static inline std::array fieldNames{FIELD_NAME(f), FIELD_NAME(i), FIELD_NAME(ch) };
};

int main() {
    C c1;

    fact_to_tuple<C>::Fields tuple;
    std::memcpy(&tuple, &c1, sizeof(C));
    // reverse order
    assert(std::get<0>(tuple) == c1.f);
    assert(std::get<1>(tuple) == c1.i);
    assert(std::get<2>(tuple) == c1.ch);
}
