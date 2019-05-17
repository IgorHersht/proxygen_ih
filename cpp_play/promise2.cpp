#include <thread>
#include <future>
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <functional>
#include <utility>

std::promise<void> p;
void doSomething() {
	int i =1;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	i = 2;
	p.set_value();
}

int main()
{
	std::thread th(doSomething);

	std::future<void> f(p.get_future());


	th.join();
	return 0;
}
