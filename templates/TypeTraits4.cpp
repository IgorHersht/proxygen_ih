
#include <iostream>


struct true_type { static constexpr bool value = true; };
struct false_type { static constexpr bool value = false; };

template<typename T> struct is_int :false_type {};
template<> struct is_int<int> :true_type {};

template<typename ... Ts > struct is_function :false_type {};
template<typename R, typename ... Args > struct is_function<R(Args ...)> :true_type {};


int main()
{

	std::cout << is_int<int> ::value << std::endl;
	std::cout << is_int<double> ::value << std::endl;
	std::cout << is_function<double> ::value << std::endl;
	std::cout << is_function<void()> ::value << std::endl;

	return 0;
}
