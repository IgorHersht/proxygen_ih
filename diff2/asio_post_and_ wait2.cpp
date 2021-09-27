#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <iostream>
#include <functional>
#include <future>
#include <iostream>
#include <iostream>
#include <iomanip>
#include <ctime>

void foo() {
	std::cout << "foo sleep" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "foo sleep" << std::endl ;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "foo sleep" << std::endl ;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "foo sleep" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

inline void postAndWait(boost::asio::io_context &context) {
	//boost::asio::post(context, foo);
	boost::asio::post(context, boost::asio::use_future(foo)).get();
	std::cout << "postAndWait" << std::endl ;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
	boost::asio::io_context context;
	
	std::thread th2([&context]() {postAndWait(context); });
	std::thread th([&context]() {context.run(); });

	th2.join();
	th.join();

}