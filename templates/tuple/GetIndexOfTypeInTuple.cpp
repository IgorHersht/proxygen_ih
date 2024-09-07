#include <tuple>
#include <concepts>

template <typename Tuple, typename T>consteval int GetIndexOfTypeInTuple() {
  return []<typename... Ts>(std::tuple<Ts...> ){
    int index = 0;
    ((std::is_same_v<Ts, T> || (++index == std::tuple_size_v<Tuple>)) || ...);
    return index ;
  }(Tuple{});
}

template <typename Tuple, typename T, int I = 0> consteval int GetIndexOfTypeInTuple2(){
  constexpr int size = std::tuple_size<Tuple>();
  if constexpr (I == size){
    return size;
  } else {
    if constexpr (std::is_same_v<std::tuple_element_t<I, Tuple>, T>) {
      return I;
    }else {
      return GetIndexOfTypeInTuple2<Tuple, T, I + 1>();
    }
  }
}

/// test
struct CoolType{}; struct SomethingElse{}; struct Boing {}; struct BingBang {};
using Tuple_Type = std::tuple< CoolType,SomethingElse,Boing,BingBang >;

static_assert(GetIndexOfTypeInTuple<Tuple_Type, CoolType>() == 0);
static_assert(GetIndexOfTypeInTuple<Tuple_Type, SomethingElse>() == 1);
static_assert(GetIndexOfTypeInTuple<Tuple_Type, Boing>() == 2);
static_assert(GetIndexOfTypeInTuple<Tuple_Type, BingBang>() == 3);
// not found
static_assert(GetIndexOfTypeInTuple<Tuple_Type, int>() == std::tuple_size_v<Tuple_Type>);
static_assert(GetIndexOfTypeInTuple<Tuple_Type, float>() == std::tuple_size_v<Tuple_Type>);
// 2
static_assert(GetIndexOfTypeInTuple2<Tuple_Type, CoolType>() == 0);
static_assert(GetIndexOfTypeInTuple2<Tuple_Type, SomethingElse>() == 1);
static_assert(GetIndexOfTypeInTuple2<Tuple_Type, Boing>() == 2);
static_assert(GetIndexOfTypeInTuple2<Tuple_Type, BingBang>() == 3);
// not found
static_assert(GetIndexOfTypeInTuple2<Tuple_Type, int>() == std::tuple_size_v<Tuple_Type>);
static_assert(GetIndexOfTypeInTuple2<Tuple_Type, float>() == std::tuple_size_v<Tuple_Type>);

int main() {

}

