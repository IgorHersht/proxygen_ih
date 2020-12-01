#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS
#include <mutex>
#include <future>
#include <thread>
#include <functional>
#include <boost/thread/future.hpp>
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/sync_bounded_queue.hpp>

template< typename T>
struct ThreadPoolOrderedFuture {
    ThreadPoolOrderedFuture(std::size_t thread_count, std::size_t maxPendingTasks) :
            m_pool(thread_count),m_queue(maxPendingTasks) { }
    template <typename F> void submit(F&& f){
        static_assert(std::is_convertible_v<F, std::function<T()> >, "bad type" );
        std::packaged_task<T( )> task(std::forward<F>(f));
        m_queue.push(task.get_future());
        m_pool.submit(std::move(task));
    }
    T get(){
        return m_queue.pull().get();
    }
private:
    boost::basic_thread_pool		                            m_pool;
    boost::concurrent::sync_bounded_queue<std::future<T> > 	    m_queue;
};

//// test
#include <iostream>
#include <chrono>
#include <memory>
#include <string>
// test1
int t11(){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
    return 1;
}
int t12(){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    return 2;
}

void test1(){
    ThreadPoolOrderedFuture<int>pool(boost::thread::hardware_concurrency(), 256);
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    pool.submit(t11);
    pool.submit(t12);

    int i1 =pool.get();
    int i2 =pool.get();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    int total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "test 1"<< std::endl;
    std::cout << "received i1=" << i1<<" i2 =" << i2 << std::endl;
    std::cout << "took " << total << " ms" << std::endl;
}
// test2
std::unique_ptr<std::string> t21(std::string s, int i){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
    return std::make_unique<std::string>(s + std::to_string(i));
}

std::unique_ptr<std::string> t22(std::string s){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
    return std::make_unique<std::string>(s);
}

void test2(){
    ThreadPoolOrderedFuture<std::unique_ptr<std::string>>pool(boost::thread::hardware_concurrency(), 256);
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    pool.submit([s="t11",i = 1]{return t21(s, i); } );
    pool.submit([s="t11"]{return t22(s); } );

    std::unique_ptr<std::string> i1 =pool.get();
    std::unique_ptr<std::string> i2 =pool.get();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    int total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "test 2"<< std::endl;
    std::cout << "received i1=" << *i1<<" i2 =" << *i2 << std::endl;
    std::cout << "took " << total << " ms" << std::endl;
}

/// test3
void t31(){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
}
void t32(){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
}
void test3(){
    ThreadPoolOrderedFuture<void>pool(boost::thread::hardware_concurrency(), 256);
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::thread t1([&pool]{pool.submit(t31); pool.submit(t31); } );
    std::thread t2([&pool]{pool.get(); pool.get(); } );
    t1.join(); t2.join();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    int total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "test 3"<< std::endl;
    std::cout << "took " << total << " ms" << std::endl;

}



int main()
{
    test1();
    test2();
    test3();

    return 0;
}

/*
test 1
received i1=1 i2 =2
took 1001 ms
test 2
received i1=t111 i2 =t11
took 1000 ms
test 3
took 1000 ms
 */

