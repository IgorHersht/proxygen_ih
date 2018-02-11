

#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/executors/serial_executor.hpp>

#include <memory>
#include <chrono>
boost::basic_thread_pool ea1(10);
boost::serial_executor ea2(ea1);

//// test
#include <iostream>
#include <boost/thread/concurrent_queues/sync_queue.hpp>
std::string s1;
std::string s2;
boost::sync_queue<std::string >  queue_in;



void in1()
{
	try {
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
		s1 = "111111111111111";
		queue_in.push(s1);
	}catch (...) {

	}
}

void in2()
{
	try {
		boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
		s2 = "222222222";
		queue_in.push(s2);
	}
	catch (...) {

	}
}


int main()
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	ea2.submit(in1);
	ea2.submit(in2);
	std::string ss1_in;
	queue_in.pull(ss1_in);
	std::string ss2_in;
	queue_in.pull(ss2_in);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	int total =  std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	std::cout << "took " <<total << " ms"<< std::endl;
	std::cout << "ss1_in =" <<ss1_in<< std::endl;
	std::cout << "ss2_in =" <<ss2_in<< std::endl;



}

/*
took 1500 ms
ss1_in =111111111111111
ss2_in =222222222

 *
 */

