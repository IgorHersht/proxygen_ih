#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <iostream>
#include <exception>
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<list>
#include<vector>
#include<queue>
#include<algorithm>
#include <type_traits>



template <bool B, class T = void> struct enable_if {};

template <class T> struct enable_if<true, T> { typedef T type; };

template <class T>
typename enable_if<std::is_integral<T>::value, bool>::type
is_odd(T i) { return bool(i % 2); }

template <class T, class X = typename enable_if<std::is_integral<T>::value, bool>::type >
bool
is_even(T i) { return !bool(i % 2); }

// A is enabled via a template parameter
template<class T, class Enable = void>
class A; // undefined

template<class T>
class A<T, typename enable_if<std::is_floating_point<T>::value >::type> {
}; // note: for this use case, static_assert may be more appropriate


template<class T>
class IntType{
	static_assert(std::is_integral<T>::value, "bad type");
};

int main(int, char*[])
{
	bool b1 = is_odd(5);
	bool b2 = is_even(5);
	A<float> a;
	//A<char> a2; //error
	IntType<long> l1;
	// IntType<double> d1; //error


	return 0;
}
