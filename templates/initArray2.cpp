#include <array>
#include <algorithm>
template<typename T, std::size_t Size, T value > constexpr auto initArray() {
  std::array<T, Size> ar{};
  std::fill(ar.begin(), ar.end(), value );
  return ar;
}

template<typename T, std::size_t Size, T value > static constexpr std::array<T, Size> ArrayInitilizer =   []() {
  std::array<T, Size> ar{};
  std::fill(ar.begin(), ar.end(), value );
  return ar;
}();

int main() {
  constexpr std::array<char, 5> ar {'x', 'x', 'x', 'x', 'x'};
  constexpr std::array<char, 5> ar2 = initArray<char,5,'x'>();
  constexpr std::array<char, 5> ar3= ArrayInitilizer<char,5,'x'>;

  static_assert(ar == ar2);
  static_assert(ar == ar3);
}
