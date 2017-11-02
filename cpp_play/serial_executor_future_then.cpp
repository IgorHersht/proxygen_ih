#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS

#include <boost/thread/future.hpp>
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <mutex>
#include <iostream>


std::mutex m_io_m;
void p1()
{
  boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
  std::lock_guard<std::mutex> lk(m_io_m);
  std::cout << "A" <<std::flush;
}

void p2()
{
	boost::this_thread::sleep_for(boost::chrono::milliseconds(20));
	std::lock_guard<std::mutex> lk(m_io_m);
	std::cout << "B" <<std::flush;
}

boost::basic_thread_pool sp(4);

void submit_serial_once()
{
	boost::future<void> f1 = boost::async(sp, &p1);
	boost::future<void> f2 = f1.then(sp,
				[](boost::future<void> f){f.wait(); p2(); }
		);
	f2.wait();
}


void submit_serial()
{
  for (int i = 0; i < 3; ++i) {
	  submit_serial_once();
  }

}

int main()
{
	submit_serial();
  return 0;
}

