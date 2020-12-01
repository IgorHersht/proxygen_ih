#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS
//#define BOOST_THREAD_USES_LOG
#define BOOST_THREAD_USES_LOG_THREAD_ID
#include <boost/thread/detail/log.hpp>

#include <boost/thread/future.hpp>
#include <boost/thread/future.hpp>
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/executor.hpp>
#include <cassert>

int p1(){

  boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
  return 1;
}

int p2( int i){

  boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
  return  i + 2;
}



int p3( int i){

  boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
  return  i + 3;
}

int p4( int i){

  boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
  return  i + 4;
}


 boost::basic_thread_pool pl1(5);
 boost::basic_thread_pool pl2(3);
 boost::basic_thread_pool pl3(6);



 void thread_pool_then_shared(){
	 boost::shared_future<int> f1 = boost::async(boost::launch::async, &p1); // start p1 in a new thread
	 boost::future<int> f2 = f1.then(pl1, // get result from p1 and start p2 in executor pl1
			[](boost::shared_future<int> f){ return p2(f.get()); }

	);
	 boost::future<int> f3 = f1.then(pl2, // get result from p1 and start p2 in executor pl1
	 			[](boost::shared_future<int> f){ return p3(f.get()); }

	 	);
	 boost::wait_for_all(f2,f3);

 }

int main()
{
	thread_pool_then_shared();

	return 0;
}

