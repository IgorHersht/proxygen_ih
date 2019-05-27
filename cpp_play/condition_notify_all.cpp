#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


struct Monitor {

void set() {
	{
		std::unique_lock<std::mutex> lc(m_m);
		m_flag = true;
		std::cout << "set from thread " << std::this_thread::get_id() << std::endl;
	}
	m_set.notify_all();
}

void wait() {
	std::unique_lock<std::mutex> lc(m_m);
	std::cout << "start wait from thread " << std::this_thread::get_id() << std::endl;
	m_set.wait(lc, [this] {return m_flag; });
	std::cout << "finish wait from thread " << std::this_thread::get_id() << std::endl;
}
bool m_flag{ false };
std::mutex m_m;
std::condition_variable m_set;
};

// test
Monitor monitor;

void set() {
	monitor.set();
}

void wait() {
	monitor.wait();
}


int main(int argc, char** argv) {

	std::thread t1(wait); std::thread t2(wait);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	set();

	t1.join(); t2.join();
	
	return 0;
}


