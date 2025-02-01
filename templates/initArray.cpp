#include <array>
template<typename T, std::size_t Size, T value > constexpr auto initArray() {
    std::array<T, Size> ar{};
    for (std::size_t pos = 0; pos < Size; ++pos) {
       ar[pos] = value;
    }
    return ar;
}

template<typename T, std::size_t Size, T value > static constexpr std::array<T, Size> ArrayInitilizer =   []() {
        std::array<T, Size> ar{};
        for (std::size_t pos = 0; pos < Size; ++pos) {
            ar[pos] = value;
        }
        return ar;
}();

int main() {
    constexpr std::array<char, 5> ar {'x', 'x', 'x', 'x', 'x'};
    static_assert(ar == initArray<char,5,'x'>());
    static_assert(ar == ArrayInitilizer<char,5,'x'>);
}
