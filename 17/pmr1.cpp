#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdlib>  // for std::byte
#include <memory_resource>
#include "tracknew.h"
//
constexpr bool isPowerOf2(size_t n) {
    return ((n != 0) && (!(n & (n - 1))));
}

template< size_t SZ, size_t AL=std::hardware_destructive_interference_size >
static std::pmr::monotonic_buffer_resource* getStaticPool(){
    static_assert(isPowerOf2(AL), " bad size");
    static_assert(AL >= sizeof(void *), " small size");

    alignas(AL) static std::array<std::byte, SZ> buf;
    static std::pmr::monotonic_buffer_resource pool{buf.data(), buf.size()};
    return &pool;
}

///
template <typename T, size_t SZ> class SmallVector{

    std::array<T, SZ> _buf;
    std::pmr::monotonic_buffer_resource _pool{_buf.data(), SZ* sizeof(T) };
public:
    std::pmr::vector<T> _vector;
    SmallVector(std::pmr::vector<T>&& vector): _vector(&_pool){
        _vector = std::move(vector);
    }

};


int main()
{
    TrackNew::reset();
    alignas(128) std::array<int, 10> _buf;
    std::pmr::monotonic_buffer_resource _pool{_buf.data(), 40};
    std::pmr::vector<int> v1{{1,2}, &_pool};
    int i = ulong (_buf.data())%128;
    ///
    SmallVector<int,12> vv1(std::move(v1)) ;
    vv1._vector.emplace_back(3);
    vv1._vector.emplace_back(4);
    //

	std::pmr::unordered_map<int,std::string> map{{{1,"a"}, {2,"b"}},  getStaticPool<1024>() };
    map.emplace(3, "c");
    int i =1;


    TrackNew::status();
}


/*
 * 0 allocations for 0 bytes
 */

