#include <queue>
#include <atomic>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <boost/circular_buffer.hpp>

template <typename T> class MQ {
	using PTR = std::unique_ptr<T>;
	using QT = boost::circular_buffer<PTR>;
	using LT = std::unique_lock<std::mutex>;
public:	
	 MQ(size_t size) :m_c(size) {
	}

	void push(PTR p) {
		LT lc(m_m);
		m_notFull.wait(lc, [this] {return !m_c.full(); });
		m_c.push_back(std::move(p));
		m_notEmpty.notify_one();
	}

	PTR pop () {
		LT lc(m_m);
		m_notEmpty.wait(lc, [this] {return !m_c.empty(); });
		PTR p (std::move(m_c.front()));
		m_c.pop_front();
		m_notFull.notify_one();
		return p;
	}


private:
	QT				m_c;
	std::mutex      m_m;
	std::condition_variable m_notEmpty;
	std::condition_variable m_notFull;
};

MQ<int> q(10);
void push()
{
	for (int i = 0; i <20; ++i) {
		q.push(std::make_unique<int>(i));
		std::cout << "push:" << i << std::endl;
	}
}

void pop()
{
	for (int i = 0; i <20; ++i) {
		std::unique_ptr<int> p = q.pop();
		std::cout << "pop:" << *p << std::endl;
	}
}


int main(int argc, char** argv) {

	std::thread t1(pop);
	std::thread t2(push);
	t2.join();
	t1.join();
	return 0;
	
	return 0;
}

