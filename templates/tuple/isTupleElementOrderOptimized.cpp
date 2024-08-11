
#include <tuple>

// 
template <typename TT, int I = 0>
consteval int isTupleElementOrderOptimized() {
   int constexpr size = std::tuple_size_v<TT>;
   if constexpr((size == 0) || (size == 1)) {
      return 0;
   } else {
      if constexpr(I == size  ) {
         return 0;
      } else  if constexpr(I == 0 ) {
         return isTupleElementOrderOptimized<TT, I +1>();
      } else {
         using CET = std::tuple_element_t<I, TT>;
         using PET = std::tuple_element_t<I -1, TT>;
         if constexpr(alignof(CET) >= alignof(PET) ) {
            return isTupleElementOrderOptimized<TT, I +1>();
         } else {
            return I;
         }
      }
   }
}
// test
#include <array>
int main() {
   using T1 = std::tuple<char, int, char >;
   using T2 = std::tuple<int, char, char>;
   using T3 = std::tuple< char, std::array<char, 10>, char, std::array<int, 10>, int, int, float, long>;
   using T4 = std::tuple< long>;

   static_assert(isTupleElementOrderOptimized<T1>() == 2);
   static_assert(isTupleElementOrderOptimized<T2>() == 1);
   static_assert(isTupleElementOrderOptimized<T3>() == 0);
   static_assert(isTupleElementOrderOptimized<T4>() == 0);
}
