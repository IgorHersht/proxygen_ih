

#include <string>
#include <vector>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/atomic.hpp>

struct X {
	typedef boost::shared_ptr<X> PtrT;
	static boost::shared_ptr<X> instance(){
		X* tmp = m_instance.load(boost::memory_order_consume);
		if(!tmp){
			boost::mutex::scoped_lock(_mutex);
			tmp = m_instance.load(boost::memory_order_consume);
			if(!tmp){
				tmp = new X;
				m_instance.store(tmp,boost::memory_order_release);
				m_ptr = PtrT(tmp);
			}
		}
		return m_ptr;
	}
private:
	X(){ std::cout <<"X()"<< std::endl; }
	X(const X&){}
	static boost::atomic< X* >		m_instance;
	static PtrT						m_ptr;
	static boost::mutex				m_mutex;
};

boost::atomic< X* > X::m_instance;
X::PtrT			X::m_ptr;
boost::mutex X::m_mutex;

int main()
{
	boost::shared_ptr<X> ptr1 = X::instance();
	boost::shared_ptr<X> ptr2 = X::instance();
  return 0;
}

