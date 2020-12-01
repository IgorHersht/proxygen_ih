#ifndef MAKE_ALIGNED_HPP
#define MAKE_ALIGNED_HPP

#include <memory>
#include <boost/align.hpp>


template<class T>
using aligned_ptr = std::unique_ptr<T, boost::alignment::aligned_delete>;

template<class T, class... Args>
inline aligned_ptr<T> make_aligned(std::size_t align, Args&&... args)
{
	auto p = boost::alignment::aligned_alloc(align, sizeof(T) * 10 );
	int s = sizeof(*p);
	if (!p) {
       return aligned_ptr<T>();
    }

    try {
        auto q = ::new(p) T(std::forward<Args>(args)...);
        return aligned_ptr<T>(q);
    } catch (...) {
        boost::alignment::aligned_free(p);
        return aligned_ptr<T>();
    }
}

#endif

#include <iostream>

struct alignas(16) type1 {
	type1(int i):m_i{i}
	{}
	~type1(){
		m_i = 0;
	}
	int m_i;
};



int main()
{
    auto p1 = make_aligned<type1>(alignof(type1), 24 );
	bool  b1 = boost::alignment::is_aligned(alignof(type1), p1.get());
	std::cout<< "is_aligned=" << b1 <<";size of type ="<< sizeof(type1) << " total size="<< sizeof(*p1)<< std::endl;
	

  return 0;
}

