
#include <iostream>
#include <string>
#include <unordered_map>
#include <cstddef>  // for std::byte
#include <memory_resource>
#include <sys/mman.h>
#include "gen_string.h"
#include "IntegralValue2.h"

//
#ifdef BASE
void init(){
    gen_strings();
    newPagefaultCount();
}

std::unordered_map<std::string, long> _m;

void test_insert(){
    const size_t sz= inputs.size();
    for(size_t i = 0; i < sz; ++i){
        _m[inputs[i]] = i;
    }
}

void test_erase(){
    const size_t sz= inputs.size();
    for(size_t i = 0; i < sz; ++i){
        _m.erase(inputs[i]) ;
    }
}
#endif


//#ifdef PMR1
/*
 2 calls
 First call without mlock on My object just to get need pool size
 Second call: 1. resize vector on pool.needBytes() 2 use  std::pmr::monotonic_buffer_resource and mlock
 */

constexpr size_t element_size =  sizeof(std::pmr::unordered_map< uint128_t, size_t >::value_type ) + 2*sizeof(void*) + alignof(std::max_align_t); //
//constexpr size_t poll_size = 562821800 ;
constexpr size_t poll_size = IN_SIZE * element_size ; // pool size estimate
 std::vector<std::byte> buf( poll_size);

 class My : public std::pmr::monotonic_buffer_resource{
     using Supper = std::pmr::monotonic_buffer_resource;
 public:
     My(void* buffer, size_t buffer_size, memory_resource* upstream):
             Supper(buffer, buffer_size, upstream){}

     void* do_allocate(size_t bytes, size_t alignment) override
     {
         void* p =  Supper::do_allocate( bytes, alignment );
         _totalAllocated  += bytes;
         _lastAllocated = bytes;
         return p;
     }

     size_t needBytes( ){
         return _totalAllocated + _lastAllocated + alignof(std::max_align_t);

     }

     size_t _totalAllocated{};
     size_t _lastAllocated{};
     size_t _lastAlignment {};

 };
#ifdef  DEBUG
std::pmr::monotonic_buffer_resource pool{buf.data(), buf.size(), std::pmr::null_memory_resource() };
#else
//My pool{buf.data(), buf.size(), std::pmr::new_delete_resource() };
std::pmr::monotonic_buffer_resource pool{buf.data(), buf.size(), std::pmr::new_delete_resource() };
#endif

  std::pmr::unordered_map< uint128_t, size_t > _m{ &pool};
    constexpr size_t MAP_SIZE = IN_SIZE ;
  void init(){
    gen_strings();
      _m.reserve(MAP_SIZE);
      _m.max_load_factor(_m.max_load_factor()*10);//make sure no rehashing
    mlock(buf.data(), buf.size());
    newPagefaultCount();
    int i =1;
}
  void test_insert(){
        for(size_t i = 0; i < MAP_SIZE; ++i){
           auto[iter, status] = _m.emplace(AsciiIntegralValue<uint128_t>(inputs[i]), i);
           if(status){
               iter->second =i;
           }

        }
    }

void test_erase(){

    const size_t sz= inputs.size();
    for(size_t i = 0; i < sz; ++i){
        _m.erase(AsciiIntegralValue<uint128_t>(inputs[i])) ;
    }
}


//#endif


int main()
{
    std::cout << "number of iterations= " << IN_SIZE << std::endl;
    init();
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        test_insert();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        Pagefaults pf = newPagefaultCount();
        size_t total = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "insert took " << (double(total) / double(IN_SIZE)) << " nanos" << std::endl;
        print(pf);
        //std::cout << "poll_size =" << poll_size << " used= " << pool._totalAllocated << " left=" << long(poll_size) - long(pool._totalAllocated) << std::endl;
        //std::cout << "need pool size=" << pool.needBytes() <<  std::endl;
        int i =1;
    }
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        test_erase();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        Pagefaults pf = newPagefaultCount();
        size_t total = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << " erace took " << (double(total) / double(IN_SIZE)) << " nanos" << std::endl;
        print(pf);
    }

}

/* output
 * number of iterations= 10000000
 * // Base
insert took 618.065 nanos
Major-Pagefaults=0 Minor-Pagefaults=280974
 erace took 405.676 nanos
Major-Pagefaults=0 Minor-Pagefaults=8


 // PMR1
inumber of iterations= 10000000
insert took 274.497 nanos
Major-Pagefaults=0 Minor-Pagefaults=2
 erace took 337.564 nanos
Major-Pagefaults=0 Minor-Pagefaults=5

 */
