#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS
#include <future>
#include <functional>
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
/// tesst
//// test
#include <iostream>
#include <chrono>
#include <string>

int t11(){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
    return 1;
}
int t12(){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    return 2;
}

std::string t21(int i){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
    return  std::to_string(i +10);
}

std::string t22(int i){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    return std::to_string(i +20);
}

ThreadPoolOrderedFuture<int> pool1(boost::thread::hardware_concurrency(), 256);
ThreadPoolOrderedFuture<std::string> pool2(boost::thread::hardware_concurrency(), 256);

void test(){
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    pool1.submit(t11);
    pool1.submit(t12);
    pool2.submit([ ](){int i =pool1.get(); return t21(i);} );
    pool2.submit([ ](){int i =pool1.get(); return t22(i);} );
    std::string s1 = pool2.get();
    std::string s2 = pool2.get();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    int total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "test"<< std::endl;
    std::cout << "received s1=" << s1<<" s2 =" << s2 << std::endl;
    std::cout << "took " << total << " ms" << std::endl;
}

int main()
{
    test();
    return 0;
}





