#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS
#include <future>
#include <thread>
#include <functional>
#include <boost/thread/executors/basic_thread_pool.hpp>


template< typename T>
struct FutureOnPool {
    template<class Pool> FutureOnPool(Pool& pool, std::function<T()> f )  {
        pool.submit([this,f ]
                    {
                        try {
                            m_p.set_value(f());
                        }catch (...) {
                            m_p.set_exception(std::current_exception());  // store exception
                        }
                    }
        );
    }

    T get(){
        return m_p.get_future().get();
    }
private:
    std::promise<T>                 m_p;
};

//template< typename T, class Pool> FutureOnPool<T> async(Pool& pool, std::function<T()> f ){
//    FutureOnPool<std::string> fut(pool, f);
//    return std::move(fut);
//}
/// test
std::string foo(){
    return "OK";
}


int main(){
    boost::basic_thread_pool pool(2);
    //FutureOnPool<std::string> fut1 = async<std::string>(pool, foo);
    FutureOnPool<std::string> fut2(pool, foo);
    std::string s = fut2.get();
    pool.close();
    int i =1;
    return 0;
}


