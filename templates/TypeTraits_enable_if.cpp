

#include <iostream>


struct true_type { static constexpr bool value = true; };
struct false_type { static constexpr bool value = false; };

template <bool B, class T = void> struct enable_if {};
template <class T> struct enable_if<true, T> { typedef T type; };

template<typename T> struct is_integral :false_type {};
template<> struct is_integral<int> :true_type {};

template<typename T> struct is_pointer :false_type {};
template<typename T> struct is_pointer<T*> :true_type {};

template<typename ... Ts > struct is_function :false_type {};
template<typename R, typename ... Args > struct is_function<R(Args ...)> :true_type {};

template <class T> typename enable_if<is_integral<T>::value, T>::type   f(T i) {
	std::cout << "int type" << std::endl;
	return i;
}

template <class T> typename enable_if<is_pointer<T>::value, T>::type   f(T i) {
	std::cout << "pointer type" << std::endl;
	return i;
}



int main()
{

	std::cout << is_integral<int> ::value << std::endl;
	std::cout << is_integral<double> ::value << std::endl;
	std::cout << is_function<double> ::value << std::endl;
	std::cout << is_function<void()> ::value << std::endl;
	int i = 1;
	f(i);
	f(&i);

	return 0;
}

