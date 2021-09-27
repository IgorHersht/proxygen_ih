#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <forward_list>
#include <iostream>
#include <type_traits>

template<typename T, typename U = void> 
struct is_map : std::false_type {};

template<typename T>
struct is_map<T, std::void_t<typename T::key_type, typename T::mapped_type>>:std::true_type {};

class A {};

template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()),
	decltype(std::declval<T>().end())>>
	: std::true_type {};


int main()
{
	std::map<int, int> m;
	std::vector<int> v;
	constexpr bool s1 = is_map<decltype(m)>::value;
	constexpr bool s2 = is_map<decltype(v)>::value;

	std::cout << std::boolalpha;
	std::cout << is_iterable<std::vector<double>>::value << '\n';
	std::cout << is_iterable<std::map<int, double>>::value << '\n';
	std::cout << is_iterable<double>::value << '\n';
	std::cout << is_iterable<A>::value << '\n';

	int i = 0;

	return 0;
}

