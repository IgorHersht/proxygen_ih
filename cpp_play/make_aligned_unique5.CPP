#include <queue>
#include <atomic>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <boost/circular_buffer.hpp>
#include<iostream>
#include<string>
#include<atomic>

#include <memory>
#include <stdlib.h>
#include <boost/align.hpp>


constexpr bool isPowerOf2(size_t n) {
	return ((n != 0) && (!(n & (n - 1))));
}

template <typename T> struct Deleter {

	void operator()(T* ptr) {
		if (!ptr) {
			return;
		}
		ptr->~T();
		free(ptr);
	}
};
template <typename T>
using PTR = std::unique_ptr<T, Deleter<T> >;
template<typename T, size_t al, typename ... Args> PTR<T> make(Args&& ... args) {
	staic_assert(isPowerOf2()al, "wrong");
	size_t all = al;
	if (al < sizeof(void*)) {
		all = sizeof(void*);
	}
	void *buf = nullptr;
	posix_memalign(&buf, all, sizeof(T));
	if (buf) {
		return PTR<T>();
	}
	PTR<T> ptr(new(buf) T(std::forward<Args>(args) ...));
	return ptr;


}

int main(int argc, char** argv) {

	

	
	return 0;
}
