#include <algorithm>
#include <tuple>
#include <array>

template <class... Ts> static consteval size_t get_max_size() requires(sizeof...(Ts) >1)  { return std::max({sizeof(Ts)...}); }
template <class T> static consteval size_t get_max_size()   { return sizeof(T); }
template <class... Ts> static consteval size_t get_max_size(std::tuple<Ts...>  )  { return get_max_size<Ts...>(); }

template <class... Ts> static consteval int ifSortedByAlignment(std::tuple<Ts...>  ) requires(sizeof...(Ts) >1){
   constexpr size_t size = sizeof...(Ts);
   constexpr std::array<size_t,sizeof...(Ts)> ar {alignof(Ts)...};
   for(size_t pos = 1; pos <size; ++pos  ) {
     if(ar[pos] < ar[pos -1] ) {
       return pos; 
     }
   }
    return 0;
}

template <class T> static consteval int ifSortedByAlignment(std::tuple<T>  ){
    return 0;
}

//test

struct V {
   std::array<char, 16> ar;
};

int main() {
   static_assert(16 == get_max_size<char, int, V, double>());
   static_assert(8 == get_max_size<double>());
   static_assert(16 == get_max_size(std::tuple<char, int, V, double>{}));
   static_assert(8 == get_max_size(std::tuple<double>{}));


    using T1 = std::tuple<char, int, char >;
    using T2 = std::tuple<int, char, char>;
    using T3 = std::tuple< char, std::array<char, 10>, char, std::array<int, 10>, int, int, float, long>;
    using T4 = std::tuple< long>;

    static_assert(ifSortedByAlignment(std::tuple<char, int, char >{}) == 2);
    static_assert(ifSortedByAlignment(std::tuple<int, char, char>{}) == 1);
    static_assert(ifSortedByAlignment(std::tuple< char, std::array<char, 10>, V, char, std::array<int, 10>, int, int, float, long>{}) == 0);
    static_assert(ifSortedByAlignment(std::tuple< long>{}) == 0);
}

