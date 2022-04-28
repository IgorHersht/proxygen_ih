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

template<typename T> struct  Deleter {
    void operator () (T* ptr){
        if(ptr){
            ptr->~T();
            free (ptr);
        }
    }
};

template <typename T> using PTR = std::unique_ptr<T, Deleter <T>>;

template< typename T, size_t al, typename ... Args> PTR<T> make(Args&& ... args) {

    static_assert(al >= sizeof(void *), " small size");
    static_assert(isPowerOf2(al), " bad size");

    void *buf = nullptr;
    posix_memalign(&buf, al, sizeof(T));
    if (buf) {
        Deleter<T> del;
      return PTR<T> (new (buf) T{std::forward<Args>(args) ...}, del);
    }
    return PTR<T>();
}

template <typename T> using Ptr = std::unique_ptr<T, Deleter<T>>;
template <typename T, size_t al, typename ... Args> Ptr<T> make2(Args&& ... args) {
    static_assert(isPowerOf2(al), "bad");
    static_assert(al >= sizeof(void*), "bad");

    Ptr<T> ptr;
    void *buf = nullptr;
    posix_memalign(&buf, al, sizeof(T));
    if(buf){
      ptr.reset(new(buf) T{std::forward<Args>(args) ...}) ;
    }
    return ptr;

}


int main()
{

PTR<std::string> pstr = make<std::string, 128>("xxxx");
const std::string S = *pstr;
int off = (long)pstr.get()%128;

 //PTR<std::string> pstr2 = make<std::string, 4>("xxxx");
 //PTR<std::string> pstr2 = make<std::string, 12>("xxxx");
 
    return 0;
}

