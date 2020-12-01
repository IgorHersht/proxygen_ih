#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS
#include <future>
#include <functional>
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/sync_bounded_queue.hpp>

struct ThreadPoolOrdered {
  ThreadPoolOrdered(std::size_t thread_count) :
      m_pool(thread_count) { }
  template <typename F>void submit(F&& f, F&& handler){
    static_assert(std::is_convertible_v<F, std::function<void()> >, "bad type" );
    std::packaged_task<void( )> task(std::forward<F>(f));
    auto fut = task.get_future();
    m_pool1.submit([fut = std::move(fut), handler = std::forward<F>(handler)] {
      try {
        fut.wait();
        handler();
      }catch(...){}
    });
    m_pool.submit(std::move(task));
  }

private:
  boost::basic_thread_pool		                                                    m_pool;
  boost::basic_thread_pool		                                                    m_pool1{1};
};

//// test
#include <string>
#include <iostream>
#include <chrono>
boost::sync_queue<int>  queue;
void run1(){
  boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
}
void run2(){
  boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
}

void send1(){
  queue.push(1);
}
void send2(){
  queue.push(2);
}

int main()
{
  ThreadPoolOrdered pl(2);
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  pl.submit(run1,send1);
  pl.submit(run2,send2);
  int i1 = queue.pull();
  int i2 = queue.pull();
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  int total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  std::cout << "received i1=" << i1<<" i2 =" << i2 << std::endl;
  std::cout << "took " << total << " ms" << std::endl;

  return 0;
}
/*
 * received i1=1 i2 =2
took 1001 ms
 */
