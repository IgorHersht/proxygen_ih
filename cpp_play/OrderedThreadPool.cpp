

#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS

#include <boost/thread/future.hpp>
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/executor.hpp>
#include <cassert>

#include <memory>
/*
 *  thread pool with ONE thread is "ordered" - submitted first will be executed first.
 *
 */
struct BasicCpuActor {

	void start() {
		if(stopped()){
			m_pool = std::make_unique<boost::basic_thread_pool>(1);//
		}
	}

	void stop() {
		try{
			if (!stopped()) {
				m_pool->close();
				m_pool->join();
				m_pool.reset();

				}
		}catch(...){

		}
	}
	template <typename Closure> void post(Closure&& closure) {
		if (!stopped()) {
			m_pool->submit(std::forward<Closure>(closure));
		}
	}


	bool stopped() {
		return (!m_pool || m_pool->closed());
	}
private:
	std::unique_ptr<boost::basic_thread_pool> m_pool;
};


struct OrderedThreadPool{

	explicit OrderedThreadPool(unsigned thread_count = boost::thread::hardware_concurrency()+1):
		m_thread_count(thread_count){

	}
	void start() {
		if(stopped()){
			m_actor.start();
			m_pool = std::make_unique<boost::basic_thread_pool>(m_thread_count);//
		}
	}
	void stop() {
		try{
			if (!stopped()) {
				m_pool->close();
				m_pool->join();
				m_pool.reset();
				m_actor.stop();

			}
		}catch(...){

		}
	}

	template <typename ClosureIn, typename ClosureOut> void post(ClosureIn in, ClosureOut  out) {
		if (stopped()) {
			return;
		}
		boost::shared_future<void> f= boost::async(*m_pool, in);
		m_actor.post([f, out  ](){
				try{
					f.wait();
					 out();
				}catch(...){

				}

			}
		);


	}

private:
		bool stopped() {
			return (!m_pool
					|| m_pool->closed()
					|| m_actor.stopped()
				);
		}

private:
	unsigned 										m_thread_count;
	BasicCpuActor 									m_actor;
	std::unique_ptr<boost::basic_thread_pool> 		m_pool;

};

//// test
#include <iostream>
std::string s1;
std::string s2;

void in1()
{
	try {
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
		s1 = "111111111111111";
		std::cout<<"in1 done " << std::endl;
	}catch (...) {

	}
}

void in2()
{
	try {
		boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
		s2 = "2222222222222";
		std::cout<<"in2 done" << std::endl;
	}
	catch (...) {

	}
}

void out1()
{
	try {
		std::cout<<" out1 done "<< s1<< std::endl;
	}catch (...) {

	}
}

void out2()
{
	try {


		std::cout<<" out2 done "<< s2<< std::endl;
	}
	catch (...) {

	}
}


int main()
{

	//boost::basic_thread_pool bp;
	//bp.submit(p1);
	//bp.submit(p2);

	//boost::this_thread::sleep_for(boost::chrono::seconds(3000));
	////

	OrderedThreadPool op ;
	op.start();
	op.post(in1, out1);
	op.post(in2, out2);


	boost::this_thread::sleep_for(boost::chrono::seconds(5));
	op.stop();

}

/*
 * in2 done
in1 done
 out1 done 111111111111111
 out2 done 2222222222222
 *
 */



