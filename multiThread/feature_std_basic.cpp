// Copyright (C) 2014 Vicente Botet
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <boost/thread/future.hpp>

#define BOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS
//#define BOOST_THREAD_USES_LOG
#define BOOST_THREAD_USES_LOG_THREAD_ID

#include <utility>
#include <memory>
#include <string>
#include <iostream>
#include <thread>
#include <future>
#include <chrono>





int p1()
{

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  return 1;
}


void std_run_in_new_thread(){
	// with async;
	std::future<int> f1 = std::async(std::launch::async, &p1);
	int res1 = f1.get();// don't have to have get here
	// with packaged_task
	std::packaged_task<int()> pt(p1);
	std:: future<int> f2=pt.get_future();
	std::thread th1(std::move(pt)); // launch task on a thread
	th1.join();// have to join
	int res2 = f2.get();
	int j =0;

}

int main(){
// start in a new thread
	 std_run_in_new_thread();


	return 0;
}

/*

int p2(boost::future<int> f)
{
  assert(f.is_ready());
  BOOST_THREAD_LOG << "p2 <" << &f << BOOST_THREAD_END_LOG;
  BOOST_TEST(f.valid());
  int i = f.get();
  boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
  BOOST_THREAD_LOG << "p2 <" << &f << BOOST_THREAD_END_LOG;
  return 2 * i;
}

void p3(boost::future<int> f)
{
  assert(f.is_ready());
  BOOST_THREAD_LOG << "p3 <" << &f << BOOST_THREAD_END_LOG;
  BOOST_TEST(f.valid());
  int i = f.get();
  boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
  BOOST_THREAD_LOG << "p3 <" << &f << " " <<i << BOOST_THREAD_END_LOG;
  return;
}



int main()
{
  BOOST_THREAD_LOG << BOOST_THREAD_END_LOG;
  {
    boost::basic_thread_pool ex(1);
    boost::future<int> f1 = boost::async(boost::launch::async, &p1);
    BOOST_TEST(f1.valid());
    boost::future<int> f2 = f1.then(ex, &p2);
    BOOST_TEST(f2.valid());
    BOOST_TEST(! f1.valid());
    try
    {
      BOOST_TEST(f2.get()==2);
    }
    catch (std::exception& ex)
    {
      BOOST_THREAD_LOG << "ERRORRRRR "<<ex.what() << "" << BOOST_THREAD_END_LOG;
      BOOST_TEST(false);
    }
    catch (...)
    {
      BOOST_THREAD_LOG << " ERRORRRRR exception thrown" << BOOST_THREAD_END_LOG;
      BOOST_TEST(false);
    }
  }
  BOOST_THREAD_LOG << BOOST_THREAD_END_LOG;
  {
    boost::basic_thread_pool ex(1);
    boost::future<int> f1 = boost::async(boost::launch::async, &p1);
    BOOST_TEST(f1.valid());
    boost::future<void> f2 = f1.then(ex, &p3);
    BOOST_TEST(f2.valid());
    try
    {
      f2.wait();
    }
    catch (std::exception& ex)
    {
      BOOST_THREAD_LOG << "ERRORRRRR "<<ex.what() << "" << BOOST_THREAD_END_LOG;
      BOOST_TEST(false);
    }
    catch (...)
    {
      BOOST_THREAD_LOG << " ERRORRRRR exception thrown" << BOOST_THREAD_END_LOG;
      BOOST_TEST(false);
    }
  }
  BOOST_THREAD_LOG << BOOST_THREAD_END_LOG;
  {
    boost::basic_thread_pool ex(1);
    boost::future<int> f2 = boost::async(p1).then(ex, &p2);
    BOOST_TEST(f2.get()==2);
  }
  BOOST_THREAD_LOG << BOOST_THREAD_END_LOG;
  {
    boost::basic_thread_pool ex(1);
    boost::future<int> f1 = boost::async(p1);
    boost::future<int> f21 = f1.then(ex, &p2);
    boost::future<int> f2= f21.then(ex, &p2);
    BOOST_TEST(f2.get()==4);
  }
  BOOST_THREAD_LOG << BOOST_THREAD_END_LOG;
  {
    boost::basic_thread_pool ex(1);
    boost::future<int> f1 = boost::async(p1);
    boost::future<int> f21 = f1.then(ex, &p2);
    boost::future<int> f2= f21.then(&p2);
    BOOST_TEST(f2.get()==4);
  }
  BOOST_THREAD_LOG << BOOST_THREAD_END_LOG;
  {
    boost::basic_thread_pool ex(1);
    boost::future<int> f1 = boost::async(p1);
    boost::future<int> f2= f1.then(&p2).then(ex, &p2);
    BOOST_TEST(f2.get()==4);
  }
  BOOST_THREAD_LOG << BOOST_THREAD_END_LOG;
  {
    boost::basic_thread_pool ex(1);
    boost::future<int> f2 = boost::async(p1).then(ex, &p2).then(ex, &p2);
    BOOST_TEST(f2.get()==4);
  }

  return boost::report_errors();
}

*/


