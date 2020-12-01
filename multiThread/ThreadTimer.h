#ifndef _THREADTIMER_H
#define _THREADTIMER_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>


namespace timerNS{

inline void sleepMilliSeconds  (long mlSecs){
	//boost::this_thread::sleep(boost::posix_time::milliseconds(mlSecs)); 
    //OK wit the the latest boost but does not work with 1.33 boost
    // Don't want to use boost::thread::sleep(const xtime& xt) because
    // need to use nanoseconds which is not portable and expensive
    // Using the system call
    usleep(1000 *  mlSecs);

}

typedef void(*onMaxCntT)();
 
class MaxCounter : private boost::noncopyable {    
  typedef boost::mutex::scoped_lock lock;
public:
  MaxCounter(int maxCnt, onMaxCntT onMaxCnt ): 
    m_initCnt(-1), 
    m_maxCnt(maxCnt), 
    m_onMaxCnt(onMaxCnt), 
    m_cnt(m_initCnt) {
  }
  
  void reset(){
    lock lk(m_mutex);
    m_cnt = m_initCnt;
  }
  
  bool increment(){  
    {lock lk(m_mutex);    
      if(m_cnt ==  m_initCnt) return false;              
      if(m_cnt < m_maxCnt) {
  	    m_cnt++;
        return true;
      }        		
      m_cnt = m_initCnt;
    }// end of  lock lk(m_mutex);
       
    m_onMaxCnt();
    return false;		
  }
  
  bool start(){
    lock lk(m_mutex);
    if(m_cnt !=  m_initCnt) return false;// error condition - start called when a counting in progress 
    m_cnt = 0;
    return true;
  }
private:
	boost::mutex        m_mutex;
	const int			m_initCnt;
	const int			m_maxCnt;
	onMaxCntT			m_onMaxCnt;
	int					m_cnt;
};

struct Timer{
	Timer(long resolutionMlSec, int maxCnt, onMaxCntT onMaxCnt  ):
	m_resolutionMlSec(resolutionMlSec),   
	m_cnt(maxCnt, onMaxCnt) {}
  
   bool start(){ return m_cnt.start(); }
   void end(){ m_cnt.reset(); }
   void threadEnrty(){
  	while(true){
  		sleepMilliSeconds(m_resolutionMlSec);
  		m_cnt.increment();
  	}
  }
private:
  int            m_resolutionMlSec;
  MaxCounter     m_cnt;
};

struct ThreadTimer {
  
  ThreadTimer(int resolutionMlSec, int maxCnt, onMaxCntT onMaxCnt  ):   
  m_timer(resolutionMlSec, maxCnt, onMaxCnt), 
  m_thread(boost::bind(&Timer::threadEnrty, boost::ref(m_timer)  ) ) {}
  
   bool start(){ return m_timer.start(); }
   void end(){ m_timer.end(); }
   ~ThreadTimer() { m_thread.join(); }	
private:
  Timer         m_timer;
  boost::thread m_thread;
};
typedef boost::shared_ptr<ThreadTimer> ThreadTimerPtr;

}//namespace timerNS




#endif 