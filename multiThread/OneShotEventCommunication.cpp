#include <thread>
#include <future>


std::promise<void> p;
void doSomething() {
   try {
	   std::this_thread::sleep_for(std::chrono::seconds(5));
	   p.set_value();
  
	}catch (...) {
		p.set_exception(std::current_exception());  // store exception
	}
}

int main()
{
  std::thread th(doSomething);

  p.get_future().wait();
  th.join();
  return 0;
}
