#include <tuple>
#include <iostream>

template<typename Tp> void printTuple( const Tp& tp) {
    std::apply([](auto&&... args) {((
            std::cout << args << " "
    ), ...);}, tp);
}

std::tuple< char, int,  float, double > from { 'x', 1, 4.4f, 7.8   };
int main()
{
    printTuple(from);
    return 0;
}
