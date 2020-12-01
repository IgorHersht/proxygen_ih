


#define BOOST_THREAD_VERSION 4
#define BOOST_THREAD_PROVIDES_EXECUTORS

#include <boost/thread/future.hpp>
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/executor.hpp>
#include <cassert>

#include <memory>
#include <chrono>
/*
*  thread pool with ONE thread is "ordered" - submitted first will be executed first.
*
*/
struct BasicCpuActor {

	void start() {
		if (stopped()) {
			m_pool = std::make_unique<boost::basic_thread_pool>(1);//
		}
	}

	void stop() {
		try {
			if (!stopped()) {
				m_pool->close();
				m_pool->join();
				m_pool.reset();

			}
		}
		catch (...) {

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


struct OrderedThreadPool {

	explicit OrderedThreadPool(unsigned thread_count = boost::thread::hardware_concurrency() + 1) :
		m_thread_count(thread_count) {

	}
	void start() {
		if (stopped()) {
			m_actor.start();
			m_pool = std::make_unique<boost::basic_thread_pool>(m_thread_count);//
		}
	}
	void stop() {
		try {
			if (!stopped()) {
				m_pool->close();
				m_pool->join();
				m_pool.reset();
				m_actor.stop();

			}
		}
		catch (...) {

		}
	}

	template <typename ClosureIn, typename ClosureOut> void post(ClosureIn in, ClosureOut  out) {
		if (stopped()) {
			return;
		}
		boost::shared_future<void> f = boost::async(*m_pool, in);
		m_actor.post([f, out]() {
			try {
				f.wait();
				out();
			}
			catch (...) {

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
#include <boost/thread/concurrent_queues/sync_queue.hpp>
std::string s1;
std::string s2;
boost::sync_queue<std::string >  queue_in;
boost::sync_queue<std::string >  queue_out;


void in1()
{
	try {
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
		s1 = "111111111111111";
		queue_in.push(s1);
	}
	catch (...) {

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

void out1()
{
	queue_out.push(s1);

}

void out2()
{
	queue_out.push(s2);

}


int main()
{


	OrderedThreadPool op;
	op.start();
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	op.post(in1, out1);
	op.post(in2, out2);

	std::string ss1_in;
	queue_in.pull(ss1_in);
	std::string ss2_in;
	queue_in.pull(ss2_in);


	std::string ss1_out;
	queue_out.pull(ss1_out);
	std::string ss2_out;
	queue_out.pull(ss2_out);


	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	int total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();


	std::cout << "took " << total << " ms" << std::endl;
	std::cout << "ss1_in =" << ss1_in << std::endl;
	std::cout << "ss2_in =" << ss2_in << std::endl;

	std::cout << "ss1_out =" << ss1_out << std::endl;
	std::cout << "ss2_out =" << ss2_out << std::endl;

	op.stop();

}
