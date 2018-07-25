#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <iostream>


template<typename T> void foo(T t) {

	if constexpr(std::is_integral_v<T>) {
		std::cout << "int" << std::endl;
	}
	else if constexpr(std::is_floating_point_v<T>) {
		std::cout << "float" << std::endl;
	}
	else {
		static_assert(false, "bad type"); // always asserts (even if discarded)
	}
}




int main()
{
	foo(5);
	foo(5.4);
	//foo(""); // error


	return 0;
}

