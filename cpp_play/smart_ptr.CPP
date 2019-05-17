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

struct RC{


  void inc(){
     ++m_c;
  }
  size_t dec(){
    return --m_c;
  }

private:
  std::atomic<size_t> m_c{1};
};

template<typename T>  struct SP{
  SP(){}
  ~SP() {
    dec();
  }

  SP(T* data):m_data(data), m_rc(new RC){
  }

  SP(const SP& sp):m_data(sp.m_data), m_rc(sp.m_rc){
    m_rc.inc();
  }
  SP& operator = (const SP& sp){
    if(this == &sp){
      return *this;
    }
    dec();
    m_data = sp.m_data;
    m_rc = sp.m_rc;
    m_rc->inc();
    return *this;
  }

private:
  void dec(){
   if( m_rc && !m_rc->dec()){
     delete m_data;
     delete m_rc;
   }
  }
private:
  T* m_data{nullptr};
  RC* m_rc{nullptr};
};

int main(){




  return 0;
  }
int main() {

}
