#include <algorithm>

template <class... T>
static constexpr size_t get_max_size() {
    return std::max({sizeof(T)...});
}
template <class... T>
static constexpr size_t get_max_align() {
    return std::max({alignof(T)...});
}

//test
#include <array>
struct V {
    std::array<char, 16> ar;
};

int main() {
   constexpr int i = get_max_size<char, int, V, double>();
    constexpr int k = get_max_align<char, double, int, V>();
    int j =1;
}
