
#include <memory>
#include <stdlib.h>
#include <boost/align.hpp>


constexpr bool isPowerOf2(size_t n) {
	return ((n != 0) && (!(n & (n - 1))));
}

template <size_t alignnemt> size_t getAlignnemt() {
	static_assert(isPowerOf2(alignnemt), "Alignnemt should be power of 2");
	return(alignnemt >= sizeof(void*)) ? alignnemt : sizeof(void*);
}


template <typename T> struct AlignedDeleter {
	void operator()(T* ptr) {
		if (!ptr) {
			return;
		}
		ptr->~T();
		boost::alignment::aligned_free(ptr);// just free on Linux
	}
};

template <typename T> using AlignePtr = std::unique_ptr<T, AlignedDeleter <T> >;
template <typename T, size_t Alignnemt, typename ... Args> inline AlignePtr<T> make_aligne_unique(T, Args&& ... args) {
	static_assert(isPowerOf2(Alignnemt), "Alignnemt should be power of 2");
	size_t alignnemt = getAlignnemt<Alignnemt>();
	AlignedDeleter<T> del;
	AlignePtr<T> ptr(nullptr, del);
	void *buf = boost::alignment::aligned_alloc(alignnemt, sizeof(T));// // just free on Linux aligned_alloc on Linux
	if (buf) {
		ptr.reset(new (buf) T(std::forward<Args>(args) ...));
	}
	return ptr;
}



int main() {
	{
		auto ptr1 = make_aligne_unique<int, 32>(25);
		auto ptr2 = make_aligne_unique<int, 2>(25);
		//auto ptr3 = make_aligne_unique<int, 12>(25);
	}
	int i = 1;

}
