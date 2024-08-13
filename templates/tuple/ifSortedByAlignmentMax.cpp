#include <algorithm>
#include <tuple>
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

//test

int main() {
    struct V {
        std::array<char, 16> ar;
    };

   static_assert(16 == get_max_size<char, int, V, double>());
   static_assert(8 == get_max_size<double>());
   static_assert(16 == get_max_size((std::tuple<char, int, V, double>*)nullptr));
   static_assert(8 == get_max_size((std::tuple<double>*)nullptr));

    static_assert(8 == get_max_alignment<char, int, V, double>());
    static_assert(4 == get_max_alignment<int>());
    static_assert(4 == get_max_alignment((std::tuple<char, int, V>*)nullptr));
    static_assert(2 == get_max_alignment((std::tuple<short>*)nullptr));

    constexpr auto ar1 = getAlignments((std::tuple< char, std::array<char, 10>, V, char, std::array<int, 10>, int, int, float, long>*)nullptr);
    static_assert(ar1 == std::array<size_t,9>{1,1 ,1, 1, 4, 4, 4, 4, 8});

    constexpr auto ar2 = getAlignments< char, std::array<char, 10>, V, char, std::array<int, 10>, int, int, float, long>();
    static_assert(ar2 == std::array<size_t,9>{1,1 ,1, 1, 4, 4, 4, 4, 8});

    static_assert(ifSortedByAlignment< char, char, short,int, int,  double, double >());
    static_assert(!ifSortedByAlignment< char, double, double, int, int, short,  char>());
    static_assert(ifSortedByAlignment<long>());

    static_assert(ifSortedByAlignment((std::tuple<char, char, short,int, int,  double, double >*)nullptr));
    static_assert(!ifSortedByAlignment( ( std::tuple< char, double, double, int, int, short,  char>*)nullptr));
    static_assert(ifSortedByAlignment((std::tuple< long>*)nullptr));
}

