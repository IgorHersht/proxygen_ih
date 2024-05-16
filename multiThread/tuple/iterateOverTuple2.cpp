#include <tuple>
#include <iostream>

int main()
{
    std::tuple t{42, 'a', 4.2}; // Another C++17 feature: class template argument deduction
    std::apply([](auto&&... args) {(
            (
                    (++args),
                    (std::cout << args  << '\n')

            ), ...);}, t);
}
