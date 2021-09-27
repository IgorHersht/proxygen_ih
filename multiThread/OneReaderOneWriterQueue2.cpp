#include <atomic>
#include <array>

template<typename T, std::size_t Size>
struct OneReaderOneWriterQueue {
  bool push(const T & value)
  {
    std::size_t h = _head;
    std::size_t t = _tail;
    std::size_t nh = next(h);
    if( nh == t){
      return false;
    }
    _ring[nh] = value;
    _head = nh;
    return true;
  }

  bool pop(T & value)
  {
    std::size_t t = _tail;
    std::size_t h = _head;
    if( h == t){
      return false;
    }
    value = t;
    _tail = next(t);
    return true;
  }
private:
  std::size_t next(std::size_t current)
  {
    return (current + 1)%Size;
  }
 alignas(std::hardware_destructive_interference_size) std::array<T, Size> _ring{};
 alignas(std::hardware_destructive_interference_size) std::atomic<std::size_t> _head{};
 alignas(std::hardware_destructive_interference_size)std::atomic<std::size_t> _tail{};
};

//// test
#include <iostream>
OneReaderOneWriterQueue<int,5> q;
void push(){
  for( int i = 0; i < 10; ++i){
    q.push(i);
  }
}

void pop(){
  for( int i = 1; i < 10; ++i){
    int j =0;
    q.pop(j);
    std::cout << j << std::endl;
  }
}

int main()
{
  push();
  pop();
  return 0;
}



