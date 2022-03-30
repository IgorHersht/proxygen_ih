T#include <type_traits>
#include <iostream>
template<class> inline constexpr bool always_false_v = false;
template<typename T> void foo(T t) {

    if constexpr(std::is_integral_v<T>) {
        std::cout << "int" << std::endl;
    } else if constexpr(std::is_floating_point_v<T>) {
        std::cout << "float" << std::endl;
    } else  {
		static_assert( always_false_v<T>, "foo cannot be instatoeted with this type");
    }
}


int main()
{
    foo(5);
    foo(5.4);
    //foo((char*)0); // error
    return 0;
}

