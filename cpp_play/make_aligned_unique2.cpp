
#include <memory>
#include <boost/align/aligned_delete.hpp>

constexpr bool isPowerOf2(unsigned long n){
	return ( (n !=0 )&& (!(n & (n -1))) );
}



template <typename T, typename... Args > inline auto make_aligned_unique(size_t alignment, Args&&... args ){

	auto Deleter = [](T* ptr){
		if(!ptr){
			return;
		}
		boost::alignment::aligned_delete del;
		del(ptr);
	};
	std::unique_ptr<T, decltype(Deleter)> ptr(nullptr, Deleter);
	if(!isPowerOf2(alignment)){
		return ptr;
	}
	void* buf = boost::alignment::aligned_alloc(alignment, sizeof(T));
	if(buf){
		ptr.reset(new(buf) T(std::forward<Args>(args)...));
	}
	return ptr;
}

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

	for(int i = 0; i < 20; ++i){
		auto p1 = make_aligned_unique<char>(4, i);
		int diff = size_t(p1.get())%4;
		if(diff){
			std::cout <<"error"<< std::endl;
			return 1;
		}
	}
	for(int i = 0; i < 20; ++i){
			auto p1 = make_aligned_unique<int>(256, i);
			int ii = *p1;
			int diff = size_t(p1.get())%256;
			if(diff){
				std::cout <<"error"<< std::endl;
				return 1;
			}
	}

	auto p1 = make_aligned_unique<char>(3,'c');
	const char* ptr = p1.get();
	if(ptr){
		std::cout <<"error"<< std::endl;
		return 1;
	}
	for(int i = 0; i < 20; ++i){
		auto p1 = make_aligned_unique<C>(256, 3);
		int diff = size_t(p1.get())%256;
		if(diff){
			std::cout <<"error"<< std::endl;
			return 1;
		}
	}


	std::cout <<"OK"<< std::endl;


	return 0;
}
}

