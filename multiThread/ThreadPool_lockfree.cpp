#include <vector>
#include <iostream>
#include <memory>
#include<string>
#include <queue>
#include <variant>
#include <thread>
#include <atomic>
#include <functional>
#include <boost/lockfree/queue.hpp>
#include <boost/chrono/chrono.hpp>

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

struct ThreadPool {
    using Task = std::function<void()>;
    using Deleter = std::function<void(Task*)>;// need to have trivial types on queue
    ThreadPool(size_t queue_size, Deleter deleter, size_t thread_count = std::thread::hardware_concurrency()):
            _tasks(queue_size), _deleter(deleter) {
        try{
            for(size_t  i = 0; i < thread_count; ++i){
                _threads.emplace_back(  [this]{worker_thread(); });
            }
        }catch(...){
            _done=true;
            throw;
        }
    }

    void done(){
        _done = true;
    }

    void submit(Task& task ){
        if(_done){
            return;
        }
        while(!_tasks.push(&task)){
            std::this_thread::yield();
        }
    }

    ~ThreadPool(){
        _done=true;
    }
private:
    void worker_thread() {
        while(!_done) {
            Task* taskPtr = nullptr;
            if(_tasks.pop(taskPtr))
            {
                try{
                    (*taskPtr)();
                    _deleter(taskPtr);
                }catch(...){
                }
            }else{
                std::this_thread::yield();
            }
        }
    }
private:
    boost::lockfree::queue<Task*>        _tasks;
    Deleter                             _deleter;
    std::vector<std::thread>            _threads;
    std::atomic<bool>                   _done{false};
    join_threads                        _joiner{_threads};
};

// test
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

void deleter(std::function<void()>* taskPtr){
 //   delete taskPtr;
}
ThreadPool pool(16, deleter);
int main() {
    {

        ThreadPool::Task task(task1);
        pool.submit(task);
        std::this_thread::sleep_for(std::chrono::seconds (10));
        int i =1;
    }
    int i =1;
}
