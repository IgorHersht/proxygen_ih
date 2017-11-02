
#include <memory>
#include <stdlib.h>
#include <type_traits>

constexpr bool isPowerOf2(size_t n){
	return ( (n !=0 )&& (!(n & (n -1))) );
}

/*
 * template <typename T, typename... Args > inline auto make_aligned_unique(size_t alignment, Args&&... args )
 * creates on heap object if type T, with constructor argunets args.
 * The object aligned according to alignment
 * Returns std::pair( ptr, rc)
 * where ptr - a std::unique_ptr to the object
 * rc - a return code
 * alignment should be power of 2 and multiple of sizeof(void *)
 *
 * return codes
[EINVAL]
    The value of the alignment parameter is not a power of two.
[ENOMEM]
    There is insufficient memory available with the requested alignment.
 */

template <typename T, typename... Args > inline auto make_aligned_unique(size_t alignment, Args&&... args ){

	auto Deleter = [](T* ptr){
		if(!ptr){
			return;
		}
		ptr -> ~T();
		free(ptr);
	};
	std::unique_ptr<T, decltype(Deleter)> ptr(nullptr, Deleter);

	if(!isPowerOf2(alignment)){
		return  ptr;
	}

	if (alignment < sizeof(void*)) {
        	alignment = sizeof(void*);
    	}


	void* buf = nullptr;
	int rc = posix_memalign(&buf, alignment, sizeof(T) );
	if(rc){
		if(buf){
			free(buf);
		}
		return ptr;
	}

	ptr.reset(new(buf) T(std::forward<Args>(args)...));// create object on the buffer
	return ptr;
}

////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> struct DefaultDeleter{
	void operator()(T* ptr){
		if(!ptr){
			return;
		}
		ptr -> ~T();
		free(ptr);
	}
};


template <typename T, size_t Alignment, template<typename> class DeleterCT = DefaultDeleter  > struct AlignedUnique{

	typedef DeleterCT<T> DeleterT;
	typedef std::unique_ptr<T, DeleterT > PtrT;

	static_assert(isPowerOf2(Alignment), "Alignment should be power of 2");
	static_assert( Alignment % sizeof(void*) == 0, "Alignment should be multiple of sizeof(void *)");

	template <typename... Args > static auto make( Args&&... args ){
		DeleterT deleter;
		PtrT  ptr(nullptr, deleter);
		void* buf = nullptr;
		int rc = posix_memalign(&buf, Alignment, sizeof(T) );
		if(rc){
			if(buf){
				free(buf);
			}
			return ptr;
		}

		ptr.reset(new(buf) T(std::forward<Args>(args)...));// create object on the buffer
		return ptr;
	}

};

//// test
#include <iostream>
struct C{
	C(int i): m_data( new int(i) ){}

	~C(){
		std::cout << "~C() "<<std::endl;
	}
	std::unique_ptr<int> m_data;

};


int main()
{

	for(int i = 0; i < 100; ++i){

			if(isPowerOf2(i)){
				std::cout <<"isPowerOf2 "<<i <<std::endl;
			}
	}

	for(int i = 0; i < 20; ++i){
		auto p1 = make_aligned_unique<char>(4, i);
		int diff = size_t(p1.get())%4;
		if(diff){
			std::cout <<"error"<< std::endl;
		}
	}

	for(int i = 0; i < 20; ++i){
			auto p1 = make_aligned_unique<int>(256, i);
			int ii = *p1;
			int diff = size_t(p1.get())%256;
			if(diff){
				std::cout <<"error"<< std::endl;
			}
	}

///////////////////////////

	for(int i = 0; i < 20; ++i){
			auto p1 = AlignedUnique<C, 256>::make(3);
			int diff = size_t(p1.get())%256;
			if(diff){
				std::cout <<"error"<< std::endl;
			}
		}

	for(int i = 0; i < 20; ++i){
		auto p1 = AlignedUnique<char, 8>::make(i);
		int diff = size_t(p1.get())%8;
		if(diff){
			std::cout <<"error"<< std::endl;
		}
	}

	for(int i = 0; i < 20; ++i){
			auto p1 = AlignedUnique<int,256>::make(i);
			//int ii = *p1;
			int diff = size_t(p1.get())%256;
			if(diff){
				std::cout <<"error"<< std::endl;
			}
	}


	for(int i = 0; i < 20; ++i){
			auto p1 = AlignedUnique<C,256>::make( 3);
			int diff = size_t(p1.get())%256;
			if(diff){
				std::cout <<"error"<< std::endl;
			}
		}


	return 0;

}

