
#include <vector>
#include <iostream>
#include <memory>
#include<string>
#include <queue>
#include <variant>
#include <thread>
#include <atomic>
#include <functional>
#include <boost/thread/sync_bounded_queue.hpp>


class ThreadPool {
  struct join_threads
  {
    join_threads(std::vector<std::thread>& th):_th(th){}
    ~join_threads(){
      try{
       for(auto& t:_th){
         if(t.joinable()){
           t.join();
         }
       }
      }catch(...){}
    };
    std::vector<std::thread>& _th;
  };
public:
  typedef std::function<void()> FuncT;
  ThreadPool(size_t queue_size, size_t thread_count = std::thread::hardware_concurrency() ): _work_queue(queue_size){
    for(size_t i = 0; i < thread_count; ++i){
      _threads.emplace_back([this]{worker_thread();});
    }
  }
  ~ThreadPool(){
    _done = true;
  }
  void submit(FuncT f){
    _work_queue.push_back(f);
  }
private:
  void worker_thread() {
    while(!_done){
      FuncT task;
      if(_work_queue.try_pull(task)){
        task();
      }else{
        std::this_thread::yield();
      }
    }
  }

private:
  std::atomic<bool>                                       _done{false};
  boost::concurrent::sync_bounded_queue<FuncT > 					_work_queue;
  std::vector<std::thread> 										            _threads;
  join_threads                                            _join_threads{_threads};
};

///test
std::mutex io_m;
void print(std::string s){
  std::lock_guard lc(io_m);
  std::cout << s<< std::endl;
}
void task1(){
  print("start task1");
  std::this_thread::sleep_for(std::chrono::seconds (1));
  print("end task1");
}
void task2(){
  print("start task2");
  std::this_thread::sleep_for(std::chrono::seconds (2));
  print("end task2");
}

int main(int argc, char** argv) {
  {
    ThreadPool pool(5, 3);
    pool.submit(task1);
    pool.submit(task2);
    std::this_thread::sleep_for(std::chrono::seconds(15));
    print("end all");
}
  int i =1;
  return 0;
}
