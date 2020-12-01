#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS

#include <boost/thread/future.hpp>
#include <boost/thread/executors/basic_thread_pool.hpp>

#include <future>
#include <thread>

int foo1(int i){

    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    return i + 1;
}

int foo2(int i){

    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
   // throw 2;
    return i + 2;
}

int foo3(int i){

    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    return i + 3;
}

boost::basic_thread_pool pl1(5);


/*
 * Calculate foo1 and foo2 and use results in foo3
 * Colecting results from foo1 and foo2 in a thread pool thread
 */
int foo1Andfoo2_foo3(int i1, int i2){
    boost::shared_future<int> f1 = boost::async(pl1, [i1]{return foo1(i1); });
    boost::shared_future<int> f2 = boost::async(pl1, [i2]{return foo2(i2); });
    boost::future<int> f3 = boost::async(pl1, [&f1, &f2]{
                                             int o1 = f1.get();
                                             int o2 = f2.get();
                                             return foo3 (o1 + o2);
                                         }
    );
    try{
        return f3.get();
    }catch(...){
        int i =1;
    }
    return 0;
}

/*
 * Calculate foo1 and use results in foo2 foo3
 * Colecting results from foo1 and foo2 in a thread pool thread
 */
int foo1_foo2Andfoo3(int i1){
    boost::shared_future<int> f1 = boost::async(pl1, [i1]{return foo1(i1); });
    boost::future<int> f2 = boost::async(pl1, [&f1]{ return foo2(f1.get()); } );
    boost::future<int> f3 = boost::async(pl1, [&f1]{ return foo3(f1.get()); } );
    try{
        int i2 = f2.get();
        int i3 = f3.get();
    }catch(...){}
    return 0;
}


int foo1Andfoo2_foo3_v2(int i1, int i2){
    std::shared_future<int> f1 = std::async(std::launch::async,[i1]{return foo1(i1);});
    std::shared_future<int> f2 = std::async(std::launch::async,[i2]{return foo2(i2);});
    std::future<int> f3 = std::async(std::launch::async, [f1 = std::move(f1), f2 = std::move(f2)]{
        return  foo3( f1.get() + f2.get() );
    });
    int i = f3.get();
    return i;
}

int main()
{
    int i1 = foo1Andfoo2_foo3(1,2);
    foo1_foo2Andfoo3(1);
    return 0;
}


