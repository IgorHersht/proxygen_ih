#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/bind.hpp>

  boost::signals2::signal<void ()> sig1;


  void Hello(){
	  std::cout << "Hello" <<std::endl;
  }
  void World(){
	  std::cout << "World" <<std::endl;
  }

void wait(int seconds){
    const boost::posix_time::time_duration sleepTime = boost::posix_time::seconds(seconds);
    boost::this_thread::sleep(sleepTime);
}

struct thread1
{

    void operator()()
    {

    	wait(1);
    	sig1();// publish from this thead
    	wait(10);
    }

};



int main()
{

	 sig1.connect(Hello); //subscribe from this thread
	 sig1.connect(World); //subscribe from this thread
	 thread1 th1Obj;
	 boost::thread thrd1(th1Obj);

	 thrd1.join();
  return 0;
};

