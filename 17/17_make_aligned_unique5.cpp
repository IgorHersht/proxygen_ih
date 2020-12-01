#include <iostream>
#include <new>
#include <memory>
#include <vector>

constexpr bool isPowerOf2(size_t n) {
  return ((n != 0) && (!(n & (n - 1))));
}

template<typename T, std::align_val_t al> struct  AlignDeleter {
  void operator () (T* ptr){
    if(ptr){
      ptr->~T();
      ::operator delete(ptr, std::align_val_t{al} );
    }
  }
};
template<typename T, std::align_val_t al>
using Ptr = std::unique_ptr<T, AlignDeleter<T,al>>;

template<typename T, std::align_val_t al, typename ... Args > Ptr<T,al> make( Args&& ... args){
  static_assert(size_t(al) >= sizeof(void *), " small size");
  static_assert(isPowerOf2(size_t(al)), " bad size");
  AlignDeleter<T,al> del;
  return Ptr<T,al>(new (std::align_val_t{al}) T(std::forward<Args>(args) ... ), del);
}
///// test
struct C{
    C(int i, int j):_i(i), _j(j){
      int k =1;
    }
    int _i;
    int _j;
    ~C(){
      int i =1;
    }
  };
int main()
{   constexpr std::align_val_t al{128};
  {
    auto ptr = make<C, al>(2,5);
    int off = size_t(ptr.get())%128;// off = 0
    std::vector<decltype(ptr)> v;
    v.emplace_back(make<C, al>(2,5));
    v.emplace_back(make<C, al>(4,5));
    int i =1;
  }
  int i =1;
  return 0;
}


