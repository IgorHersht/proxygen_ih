#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <iostream>
#include <functional>
#include <future>
#include <iostream>
#include <iostream>
#include <iomanip>
#include <ctime>

using boost::asio::thread_pool;

inline void postAndWait(thread_pool &context, std::function<void(std::promise<void> &p)> f, std::promise<void> &p) {
	boost::asio::dispatch(context, [f, &p]() { f(p); });
	std::cout << "1" << std::endl;
	p.get_future().wait();
}

void foo(std::promise<void> &p) {
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << "2" << std::endl;
	p.set_value();
}

int main()
{
	thread_pool context{ 1 };
	std::promise<void> p;
	//std::thread th([&context]() {context.run(); });
	auto t1 = std::chrono::system_clock::now();

	postAndWait(context, foo, p);
	auto t2 = std::chrono::system_clock::now();
	std::cout << "Printing took "
		<< std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count()
		<< std::endl <<std::fflush;
	int i = 0;
	context.join();	
}
