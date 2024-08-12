#include <algorithm>
#include <tuple>

template <class... Ts> static consteval size_t get_max_size() requires(sizeof...(Ts) >1)  { return std::max({sizeof(Ts)...}); }
template <class T> static consteval size_t get_max_size()   { return sizeof(T); }
template <class... Ts> static consteval size_t get_max_size(std::tuple<Ts...>  )  { return std::max({sizeof(Ts)...}); }

//test
#include <array>
struct V {
   std::array<char, 16> ar;
};

int main() {
   static_assert(16 == get_max_size<char, int, V, double>());
   static_assert(8 == get_max_size<double>());

   using TT = std::tuple<char, int, V, double>;
   constexpr TT t;
   static_assert(16 == get_max_size(t));
}
