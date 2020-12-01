#include <vector>
#include <iostream>
#include <memory>
#include<string>
#include <queue>
#include <variant>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <boost/circular_buffer.hpp>


template <typename T> struct Queue{
  explicit Queue(size_t size):_q(size){
  }
  using QT = boost::circular_buffer<T>;
  void push( T t){
    std::unique_lock lc(_m);
    while(_q.full()){ _notFull.wait(lc); }
    _q.push_back(std::move(t));
    _notEmpty.notify_one();
  }

  T pop(){
    std::unique_lock lc(_m);
    while(_q.empty()){ _notEmpty.wait(lc); };
    T t {std::move(_q.front())};
    _q.pop_front();
    _notFull.notify_one();
    return t;
  }
private:
  QT            _q;
  mutable std::mutex    _m;
  std::condition_variable   _notFull;
  std::condition_variable   _notEmpty;
};

//// test
using DT = std::unique_ptr<int>;
Queue<DT> q(10);

void pushAll(){
  for(size_t i = 0; i < 30; ++i ){
    std::this_thread::sleep_for(std::chrono::milliseconds (10));
    q.push(std::make_unique<int>(i));
  }
}

void popAll(){
  for(size_t i = 0; i < 30; ++i ){
    DT d = q.pop();
    std::cout << *d << std::endl;
  }
}


int main(int argc, char** argv) {
  std::thread t1(pushAll); std::thread t2(popAll);
  t1.join(); t2.join();

  return 0;
}

