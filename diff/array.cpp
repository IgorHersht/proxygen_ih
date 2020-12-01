

#include <experimental/array>
#include <iostream>
#include <type_traits>

template<class T, size_t N> constexpr size_t size(T (&)[N]) { return N; }// pre C++11

int main()
{
    decltype(auto) arr = std::experimental::make_array(1, 2, 3, 4, 5);
    bool is_array_of_5_ints = std::is_same<decltype(arr), std::array<int, 5>>::value;
    std::cout << "Returns an array of five ints? ";
    std::cout << std::boolalpha << is_array_of_5_ints << '\n';

    ////
    const auto ext = std::extent<int[9]>{};
    std::cout << ext << '\n'; //< implicit conversion to std::size_t
    const int ints[] = {1,2,3,4};
    std::cout << std::extent<decltype(ints)>::value << '\n'; //< array size
    ///
    std::cout << size(ints) << '\n'; //< array size

}




