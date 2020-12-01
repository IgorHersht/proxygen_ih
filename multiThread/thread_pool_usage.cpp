

#include <boost/assert.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include <wangle/concurrent/CPUThreadPoolExecutor.h>
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/future.hpp>

std::mutex io_m;
void f1(){
	std::this_thread::sleep_for(std::chrono::seconds(10));
	std::lock_guard<std::mutex> lc(io_m);
	std::cout << "f1"<< std::endl;
}
void f2(){
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::lock_guard<std::mutex> lc(io_m);
	std::cout << "f2"<< std::endl;
}



int main()
{
	//boost::basic_thread_pool pool(10);
	//pool.submit(f1);
	//pool.submit(f2);
	wangle::CPUThreadPoolExecutor pool2(10);
	pool2.add(f1);
	pool2.add(f2);
    std::this_thread::sleep_for(std::chrono::seconds(200));//



  return 0;
}
