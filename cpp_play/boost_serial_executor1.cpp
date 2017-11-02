// Copyright (C) 2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/executors/serial_executor.hpp>

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

boost::basic_thread_pool ea1(10);
boost::serial_executor ea2(ea1);

void submit_thread_pool()
{
  for (int i = 0; i < 3; ++i) {
	  ea1.submit(&p1);
  }
  for (int i = 0; i < 3; ++i) {

	  ea1.submit(&p2);
  }
}

void submit_serial()
{
  for (int i = 0; i < 3; ++i) {
	  ea2.submit(&p1);
  }
  for (int i = 0; i < 3; ++i) {

	  ea2.submit(&p2);
  }
}


int test_executor_adaptor()
{
  {
    try
    {
        std::cout << "basic_thread_pool" << std::endl;
        submit_thread_pool();
        boost::this_thread::sleep_for(boost::chrono::seconds(2));
        std::cout << "\nserial executor" << std::endl;
        submit_serial();
        boost::this_thread::sleep_for(boost::chrono::seconds(2));// make sure all done


    }
    catch (std::exception& ex)
    {
      std::cout << "ERROR= " << ex.what() << "" << std::endl;
      return 1;
    }
    catch (...)
    {
      std::cout << " ERROR= exception thrown" << std::endl;
      return 2;
    }
  }
  return 0;
}


int main()
{
  return test_executor_adaptor();
}

