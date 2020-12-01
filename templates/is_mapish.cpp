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

template<typename T, typename ... Args> struct IsMap {
	static constexpr bool value = false;
};

template<typename ... Args> struct IsMap<std::map< Args ...> > {
	static constexpr bool value = true;
};

template<typename T, typename ... Args> struct IsUnorderedMap {
	static constexpr bool value = false;
};

template<typename ... Args> struct IsUnorderedMap<std::unordered_map< Args ...> > {
	static constexpr bool value = true;
};

template<typename T, typename ... Args> struct IsMapish {
	static constexpr bool value = (IsMap<T>::value || IsUnorderedMap<T>::value);
};

template <typename T> auto sumKeys(const T& c) {
	typename T::key_type sum = typename T::key_type();
	if constexpr(IsMapish<T>::value) {
		for (const T::value_type& v : c) {
			sum += v.first;
		}
	}

	static_assert(IsMapish<T>::value, "bad");
	return sum;
}


int main()
{
	std::set<int> s{1,2};
	std::map<int, int> m{ {1,2}, {3,4} };

	//int s1 = sumKeys(s);
	int s2 = sumKeys(m);


	return 0;
}
