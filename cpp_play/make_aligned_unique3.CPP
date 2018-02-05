#ifndef MAKE_ALIGNED_HPP
#define MAKE_ALIGNED_HPP

#include <memory>
#include <array>
#include <boost/align.hpp>



	constexpr bool isPowerOf2(size_t n) {
		return ((n != 0) && (!(n & (n - 1))));
	}


	template<class T>
	using aligned_ptr = std::unique_ptr<T, boost::alignment::aligned_delete>;

	template<class T, size_t Alignment, class... Args>
	inline aligned_ptr<T> make_aligned(Args&&... args) {

		static_assert(isPowerOf2(Alignment), "Alignment should be power of 2");
		static_assert(Alignment % sizeof(void*) == 0, "Alignment should be multiple of sizeof(void *)");
		aligned_ptr<T> ptr;

		auto p = boost::alignment::aligned_alloc(align, sizeof(T));
		if (!p) {
			return ptr;
		}

		try {
			auto pt = ::new(p) T(std::forward<Args>(args)...);
			ptr.reset(pt);
		}
		catch (...) {
			boost::alignment::aligned_free(p);
		}
		return ptr;
	}




#endif
// test
typedef std::array<char, 1024> DT;
#include <iostream>




int main()
{
	{
		auto ptr1 = make_aligned<int, 32>(25);
		auto ptr2 = make_aligned<int, 2>(25);
		//auto ptr3 = make_aligne_unique<int, 12>(25);
	}
	int i = 1;


	return 0;
}

