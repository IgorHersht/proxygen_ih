

#include <boost/thread/executors/basic_thread_pool.hpp>

#include <memory>
/*
 *  thread pool with ONE thread is "ordered" - submitted first will be executed first.
 *
 */
struct BasicCpuActor {
	void start() {
		m_pool = std::make_unique<boost::basic_thread_pool>(1);//
	}
	void stop() {
		if (!stopped()) {
			m_pool->close();
			m_pool->join();
		}
	}
	template <typename Closure> void post(Closure&& closure) {
		if (!stopped()) {
			m_pool->submit(std::forward<Closure>(closure));
		}
	}

private:
	bool stopped() {
		return (!m_pool || m_pool->closed());
	}
private:
	std::unique_ptr<boost::basic_thread_pool> m_pool;
};



/// test
#include <iostream>
void p1()
{
	try {
		boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));

		std::cout<<"p1" << std::endl;
	}catch (...) {

	}
}

void p2()
{
	try {
		boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

		std::cout<<"p2" << std::endl;
	}
	catch (...) {

	}
}


int main()
{
	BasicCpuActor actor;
	actor.start();
	actor.post(p1);
	actor.post(p2);

}

