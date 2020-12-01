#include <vector>
#include <iostream>
#include <memory>
#include<string>
#include <queue>
#include <variant>
#include <thread>
#include <atomic>
#include <functional>
#include <boost/thread/concurrent_queues/sync_bounded_queue.hpp>
#include <boost/chrono/chrono.hpp>

class ThreadPool {
    struct join_threads {
        join_threads(std::vector<std::thread> &th) : _th(th) {}

        ~join_threads() {
            try {
                for (auto &t:_th) {
                    if (t.joinable()) {
                        t.join();
                    }
                }
            } catch (...) {}
        };
        std::vector<std::thread> &_th;
    };
public:
    using Task = std::function<void()>;
    ThreadPool( size_t thread_count = std::thread::hardware_concurrency(), size_t queue_size = std::thread::hardware_concurrency()):
    _tasks(queue_size){
        for(size_t  i = 0; i < thread_count; ++i){
            _threads.emplace_back(  [this]{worker_thread(); });
        }
    }
    void submit(Task task ){
        _tasks.push(std::move(task));
    }
    void stop(){
        _done = true;
        for(size_t  i = 0; i < _threads.size(); ++i){
            submit(  [this]{dummy(); });
        }
    }
    ~ThreadPool(){
        stop();
    }
private:
    void dummy(){};
    void worker_thread() {
      while(!_done){
          Task task = _tasks.pull();
          try {
              task();
          }catch(...){}
      }
    }
private:
    boost::sync_bounded_queue<Task>     _tasks;
    std::vector<std::thread>            _threads;
    join_threads                        _join_threads{_threads};
    std::atomic<bool>                   _done{false};
};
/// test
std::mutex io_m;
void print(std::string s){
    std::lock_guard lc(io_m);
    std::cout << s<< std::endl;
}

void task1(){
    print("start task1");
    std::this_thread::sleep_for(std::chrono::seconds (10));
    print("end task1");
}
int main() {
    {
        ThreadPool pool;
        pool.submit(task1);
        int i =1;
    }
    int i =1;
}

