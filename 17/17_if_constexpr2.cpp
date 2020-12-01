#include <type_traits>
#include <iostream>

template<typename T> void foo(T t) {

    if constexpr(std::is_integral_v<T>) {
        std::cout << "int" << std::endl;
    } else if constexpr(std::is_floating_point_v<T>) {
        std::cout << "float" << std::endl;
    } else  {
        static_assert( sizeof(T) == 0, "foo cannot be instatoeted with this type");
    }
}


int main()
{
    foo(5);
    foo(5.4);
    //foo((char*)0); // error
    return 0;
}

