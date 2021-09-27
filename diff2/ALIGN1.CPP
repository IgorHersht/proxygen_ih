#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>
#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <iterator>
#include <memory>
#include <array>
/*
alignof(std::max_align_t)
*/
struct alignas(16)sse_t ;
struct alignas(16) sse_t
{
  char sse_data[4];
   alignas(32) int m_i;
};
 
// the array "cacheline" will be aligned to 128-byte boundary
alignas(128) char cacheline[128];
alignas(8) int i_8;
alignas(2) int i_2;

char stor[128];



int main(int, char*[])
{
  size_t ali = alignof(int);
  size_t al_sse_t = alignof(sse_t);// 32 because of alignof(sse_t::m_i) == 32
  size_t al_data = alignof(sse_t::sse_data);
  size_t al_m_i = alignof(sse_t::m_i);
  size_t al_cacheline = alignof(cacheline);
  size_t al_i_8 = alignof(i_8);
  size_t al_i_2 = alignof(i_2);
  /*
  void* align(std::size_t alignment, std::size_t size, void*& ptr, std::size_t& space); 
  Effects: If it is possible to fit size bytes of storage aligned by alignment into the buffer pointed to by
  ptr with length space, the function updates ptr to point to the first possible address of such storage
  and decreases space by the number of bytes used for alignment. Otherwise, the function does nothing. 

  */ 

   //void *al_st = std::align(7, 10, (void*)stor, 128); // not implemented in gcc
  std::cout << std::alignment_of<float>::value << '\n';
  std::cout << std::alignment_of<decltype(al_i_8)>::value << '\n';
  decltype(al_i_8) a_i8[20];
  std::cout << std::alignment_of<decltype(a_i8)>::value << '\n';

  /*
  The type defined by std::aligned_storage<>::type can be 
  used to create uninitialized memory blocks suitable to hold 
  the objects of given type, optionally aligned stricter than their natural alignment requirement, for example on a cache or page boundary
  */
  std::aligned_storage<64, 16> ::type storage ;
  
  std::cout << "data al="<< alignof(storage) << " size=" << sizeof(storage)<< std::endl;

  return 0;
}
